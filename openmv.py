import sensor, image, time, math, pyb
from pyb import Pin, Timer, UART, LED

#the size of the image: QVGA
IMG_WIDTH  = 320
IMG_HEIGHT = 240

#--------------Camera setup-------------------
sensor.reset() # Initialize the camera sensor.
sensor.set_pixformat(sensor.GRAYSCALE) # use grayscale.
sensor.set_framesize(sensor.QVGA) # use QQVGA for speed.
sensor.skip_frames(time = 2000) # Let new settings take affect.
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
clock = time.clock() # Tracks FPS.
#--------------Camera setup finished-------------------
#
#
#
#
#
#--------------串口UART部分 START -------------------
uart = pyb.UART(3,9600,timeout_char = 1000) #串口初始化，9600bps@11.0592MHz for STC89c51
is_debug = True

#def data_format_wrapper(yaw_angle, sum_x, sum_y, cx_mean, cx, cy, is_left_angle, last_x, last_y):
def data_format_wrapper(cx_mean, cx, cy, is_turn_left, is_turn_right, is_t, is_cross):
    '''
    根据通信协议封装数据
    TODO 重新编写通信协议  与配套C解析代码

    yaw_angle,sum_x, sum_y 没有用到
    cx_mean: roi 1 2 3 对应的bolb中心的x坐标加权平均， 如果没有就补32  都没有就赋值为原来的值
    cx : roi 1 3 blob 中心坐标的加权平均， 如果一方丢失， 就赋值为另外一个  都没有就赋值为原来的值
    cy : roi 4 5 blob 中心坐标的加权平均，如果一方丢失， 就赋值为另外一个， 都没有就赋值为原来的值
    is_left_angle :代表是否有直角，！！！ 信息丢失
             其实，可以用一个字符来表示，是左转还是右转 T(T字形)  L(Left) R(Right)
    last_x, last_y 是两个直线的交叉点的坐标  改写为 intersect_x，intersect_y
    '''
    args = [
        #get_symbol(symbol), # 偏航角符号
        #abs(int(yaw_angle)), # 偏航角
        #get_symbol(sum_x) # 光流数据sum_x的符号
        ##abs(int(sum_x)), # 光流数据sum_x
        #get_symbol(sum_y), # 光流数据sum_y的符号
        #abs(int(sum_y)), # 光流数据 sum_y
        int(cx_mean), 				# x的中心，三个取样区域色块中心x坐标的平均值
        int(cx),							# 检测到T形或者十字形路口后，停车并计算圆心坐标
        int(cy),							# 同上
        #int(speed),						# 测速
        int(is_turn_left),		# 是否左转（非T非十字）
        int(is_turn_right),		# 是否右转（非T非十字）
        int(is_t),						# 是否T形路口
        int(is_cross),				# 是否十字路口
    ]
    # 将数值列表按照通信协议，转换为待发送的字符
    #info = 's%c%.2d%c%.2d%c%.2d%.2d%.2d%.2d%.2d%.2d%.2d#'%tuple(args)    #SOURCE
    info = 's%.3d%.3d%.3d%d%d%d%d#'%tuple(args)
    global is_debug
    if is_debug:
        print('s%.3d%.3d%.3d | %d%d | %d%d#'%tuple(args))
    return info
#--------------串口UART部分 END -------------------
#
#
#
#
#
#--------------定时器部分 START -------------------
is_need_send_data = False # 是否需要发送数据的信号标志
def uart_time_trigger(timer):
    '''
    串口发送数据的定时器，定时器的回调函数
    '''
    global is_need_send_data
    is_need_send_data = True


tim = Timer(4, freq=10)
tim.callback(uart_time_trigger)
#--------------定时器部分 END -------------------
#
#
#
#
#
#--------------直线与直角检测部分 START -------------------
INTERSERCT_ANGLE_THRESHOLD = (45,90)
LINE_COLOR_THRESHOLD = [(0, 128)]
ROIS = {
   'down': (0, 210, 320, 30), # 横向取样-下方 1     # 左上角是起始点，方向是向下和向右增大
   'middle': (0, 100, 320, 40), # 横向取样-中间 2   # 前两位数是区域起始坐标
   'up': (0, 0, 320, 30), # 横向取样-上方 3        # 后两位数是宽/高度
   'left': (0, 0, 30, 240), # 纵向取样-左侧 4
   'right': (290, 0, 30, 240) # 纵向取样-右侧 5
}
def find_blobs_in_rois(img):
    '''
    在ROIS中寻找色块，获取ROI中色块的中心区域与是否有色块的信息
    '''
    global ROIS
    global is_debug

    roi_blobs_result = {}  # 在各个ROI中寻找色块的结果记录
    for roi_direct in ROIS.keys():
        roi_blobs_result[roi_direct] = {
            'cx': -1,
            'cy': -1,
            'blob_flag': False
        }
    for roi_direct, roi in ROIS.items():
        blobs=img.find_blobs(LINE_COLOR_THRESHOLD, roi=roi, merge=True, pixels_area=10)
        if len(blobs) == 0:
            continue

        largest_blob = max(blobs, key=lambda b: b.pixels())
        x,y,width,height = largest_blob[:4]


        roi_blobs_result[roi_direct]['cx'] = largest_blob.cx()
        roi_blobs_result[roi_direct]['cy'] = largest_blob.cy()
        roi_blobs_result[roi_direct]['blob_flag'] = True

        if is_debug:
            img.draw_rectangle((x,y,width, height), color=(255))

    return roi_blobs_result

def visualize_result(canvas, cx_mean, cx, cy, is_turn_left, is_turn_right, is_t, is_cross):
    '''
    可视化结果
    '''
    if not(is_turn_left or is_turn_right or is_t or is_cross):
        mid_x = int(canvas.width()/2)
        mid_y = int(canvas.height()/2)
        # 绘制x的均值点
        canvas.draw_circle(int(cx_mean), mid_y, 5, color=(255))
        # 绘制屏幕中心点
        canvas.draw_circle(mid_x, mid_y, 8, color=(0))
        canvas.draw_line((mid_x, mid_y, int(cx_mean), mid_y), color=(255))

    turn_type = 'N' # 啥转角也不是

    if is_t or is_cross:
        # 十字形或者T形
        canvas.draw_cross(int(cx), int(cy), size=10, color=(255))
        canvas.draw_circle(int(cx), int(cy), 5, color=(255))

    if is_t:
        turn_type = 'T' # T字形状
    elif is_cross:
        turn_type = 'C' # 十字形
    elif is_turn_left:
        turn_type = 'L' # 左转
    elif is_turn_right:
        turn_type = 'R' # 右转

    canvas.draw_string(0, 0, turn_type, color=(0), scale=3)

#--------------直线与直角检测部分 END -------------------
#
#
#
#
#
#--------------MAIN -------------------
last_cx = 0
last_cy = 0

while True:
    LED(3).on()
    if not is_need_send_data:
        # 不需要发送数据
        continue
    is_need_send_data = False

    # 拍摄图片
    img = sensor.snapshot()
    # 创建画布
    # canvas = img.copy()
    # 为了IDE显示方便，直接在代码结尾 用IMG绘制

    lines = img.find_lines(threshold=1000, theta_margin = 50, rho_margin = 50)
    reslut = find_blobs_in_rois(img)

    # 判断是否需要左转与右转
    is_turn_left = False
    is_turn_right = False


    if (not reslut['up']['blob_flag'] ) and reslut['down']['blob_flag']:
        if reslut['left']['blob_flag']:
            is_turn_left = True
        if reslut['right']['blob_flag']:
            is_turn_right = True


    # 判断是否为T形的轨道
    is_t = False
    # 判断是否十字形轨道
    is_cross = False
    
    c_num = 0
    t_num = 0
    for roi_direct in ['up', 'down', 'left', 'right']:
        if reslut[roi_direct]['blob_flag']:
            c_num += 1
    is_cross = c_num == 4
    for roi_direct in ['left', 'right']:
        if reslut[roi_direct]['blob_flag']:
            t_num += 1
    is_t = t_num == 2        

    # cx_mean 用于确定视角中的轨道中心
    # 用于表示左右偏移量
    cx_mean = 0
    
    for roi_direct in ['up', 'down', 'middle']:
        if reslut[roi_direct]['blob_flag']:
            cx_mean += reslut[roi_direct]['cx']
        else:
            cx_mean += IMG_WIDTH / 2
    cx_mean /= 3

    # cx, cy 只有在T形区域检测出来的时候才有用，
    # 用于确定轨道中圆形的大致区域， 用于定点， 是计算圆心的一种近似方法
    # cx, cy 只有在T形区域检测出来的时候才有用，
    # 用于确定轨道中圆形的大致区域， 用于定点， 是计算圆心的一种近似方法
    cx = 0
    cy = 0

    if is_cross or is_t:
        # 只在出现十字形或者T字形才计算圆心坐标
        cnt = 0
        for roi_direct in ['up', 'down']:
            if reslut[roi_direct]['blob_flag']:
                cnt += 1
                cx += reslut[roi_direct]['cx']
        if cnt == 0:
            cx = last_cx
        else:
            cx /= cnt

        cnt = 0
        for roi_direct in ['left', 'right']:
            if reslut[roi_direct]['blob_flag']:
                cnt += 1
                cy += reslut[roi_direct]['cy']
        if cnt == 0:
            cy = last_cy
        else:
            cy /= cnt

    info = data_format_wrapper(cx_mean, cx, cy, is_turn_left, is_turn_right, is_t, is_cross)
    uart.write(info)
    #time.sleep(30)

    last_cx = cx
    last_cy = cy

    if is_debug:
        visualize_result(img, cx_mean, cx, cy, is_turn_left, is_turn_right, is_t, is_cross)

    #print(clock.fps()) # Note: Your OpenMV Cam runs about half as fast while
    # connected to your computer. The FPS should increase once disconnected.
