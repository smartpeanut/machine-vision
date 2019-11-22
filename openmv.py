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
#--------------����UART���� START -------------------
uart = pyb.UART(3,9600,timeout_char = 1000) #���ڳ�ʼ����9600bps@11.0592MHz for STC89c51
is_debug = True

#def data_format_wrapper(yaw_angle, sum_x, sum_y, cx_mean, cx, cy, is_left_angle, last_x, last_y):
def data_format_wrapper(cx_mean, cx, cy, is_turn_left, is_turn_right, is_t, is_cross):
    '''
    ����ͨ��Э���װ����
    TODO ���±�дͨ��Э��  ������C��������

    yaw_angle,sum_x, sum_y û���õ�
    cx_mean: roi 1 2 3 ��Ӧ��bolb���ĵ�x�����Ȩƽ���� ���û�оͲ�32  ��û�о͸�ֵΪԭ����ֵ
    cx : roi 1 3 blob ��������ļ�Ȩƽ���� ���һ����ʧ�� �͸�ֵΪ����һ��  ��û�о͸�ֵΪԭ����ֵ
    cy : roi 4 5 blob ��������ļ�Ȩƽ�������һ����ʧ�� �͸�ֵΪ����һ���� ��û�о͸�ֵΪԭ����ֵ
    is_left_angle :�����Ƿ���ֱ�ǣ������� ��Ϣ��ʧ
             ��ʵ��������һ���ַ�����ʾ������ת������ת T(T����)  L(Left) R(Right)
    last_x, last_y ������ֱ�ߵĽ���������  ��дΪ intersect_x��intersect_y
    '''
    args = [
        #get_symbol(symbol), # ƫ���Ƿ���
        #abs(int(yaw_angle)), # ƫ����
        #get_symbol(sum_x) # ��������sum_x�ķ���
        ##abs(int(sum_x)), # ��������sum_x
        #get_symbol(sum_y), # ��������sum_y�ķ���
        #abs(int(sum_y)), # �������� sum_y
        int(cx_mean), 				# x�����ģ�����ȡ������ɫ������x�����ƽ��ֵ
        int(cx),							# ��⵽T�λ���ʮ����·�ں�ͣ��������Բ������
        int(cy),							# ͬ��
        #int(speed),						# ����
        int(is_turn_left),		# �Ƿ���ת����T��ʮ�֣�
        int(is_turn_right),		# �Ƿ���ת����T��ʮ�֣�
        int(is_t),						# �Ƿ�T��·��
        int(is_cross),				# �Ƿ�ʮ��·��
    ]
    # ����ֵ�б���ͨ��Э�飬ת��Ϊ�����͵��ַ�
    #info = 's%c%.2d%c%.2d%c%.2d%.2d%.2d%.2d%.2d%.2d%.2d#'%tuple(args)    #SOURCE
    info = 's%.3d%.3d%.3d%d%d%d%d#'%tuple(args)
    global is_debug
    if is_debug:
        print('s%.3d%.3d%.3d | %d%d | %d%d#'%tuple(args))
    return info
#--------------����UART���� END -------------------
#
#
#
#
#
#--------------��ʱ������ START -------------------
is_need_send_data = False # �Ƿ���Ҫ�������ݵ��źű�־
def uart_time_trigger(timer):
    '''
    ���ڷ������ݵĶ�ʱ������ʱ���Ļص�����
    '''
    global is_need_send_data
    is_need_send_data = True


tim = Timer(4, freq=10)
tim.callback(uart_time_trigger)
#--------------��ʱ������ END -------------------
#
#
#
#
#
#--------------ֱ����ֱ�Ǽ�ⲿ�� START -------------------
INTERSERCT_ANGLE_THRESHOLD = (45,90)
LINE_COLOR_THRESHOLD = [(0, 128)]
ROIS = {
   'down': (0, 210, 320, 30), # ����ȡ��-�·� 1     # ���Ͻ�����ʼ�㣬���������º���������
   'middle': (0, 100, 320, 40), # ����ȡ��-�м� 2   # ǰ��λ����������ʼ����
   'up': (0, 0, 320, 30), # ����ȡ��-�Ϸ� 3        # ����λ���ǿ�/�߶�
   'left': (0, 0, 30, 240), # ����ȡ��-��� 4
   'right': (290, 0, 30, 240) # ����ȡ��-�Ҳ� 5
}
def find_blobs_in_rois(img):
    '''
    ��ROIS��Ѱ��ɫ�飬��ȡROI��ɫ��������������Ƿ���ɫ�����Ϣ
    '''
    global ROIS
    global is_debug

    roi_blobs_result = {}  # �ڸ���ROI��Ѱ��ɫ��Ľ����¼
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
    ���ӻ����
    '''
    if not(is_turn_left or is_turn_right or is_t or is_cross):
        mid_x = int(canvas.width()/2)
        mid_y = int(canvas.height()/2)
        # ����x�ľ�ֵ��
        canvas.draw_circle(int(cx_mean), mid_y, 5, color=(255))
        # ������Ļ���ĵ�
        canvas.draw_circle(mid_x, mid_y, 8, color=(0))
        canvas.draw_line((mid_x, mid_y, int(cx_mean), mid_y), color=(255))

    turn_type = 'N' # ɶת��Ҳ����

    if is_t or is_cross:
        # ʮ���λ���T��
        canvas.draw_cross(int(cx), int(cy), size=10, color=(255))
        canvas.draw_circle(int(cx), int(cy), 5, color=(255))

    if is_t:
        turn_type = 'T' # T����״
    elif is_cross:
        turn_type = 'C' # ʮ����
    elif is_turn_left:
        turn_type = 'L' # ��ת
    elif is_turn_right:
        turn_type = 'R' # ��ת

    canvas.draw_string(0, 0, turn_type, color=(0), scale=3)

#--------------ֱ����ֱ�Ǽ�ⲿ�� END -------------------
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
        # ����Ҫ��������
        continue
    is_need_send_data = False

    # ����ͼƬ
    img = sensor.snapshot()
    # ��������
    # canvas = img.copy()
    # Ϊ��IDE��ʾ���㣬ֱ���ڴ����β ��IMG����

    lines = img.find_lines(threshold=1000, theta_margin = 50, rho_margin = 50)
    reslut = find_blobs_in_rois(img)

    # �ж��Ƿ���Ҫ��ת����ת
    is_turn_left = False
    is_turn_right = False


    if (not reslut['up']['blob_flag'] ) and reslut['down']['blob_flag']:
        if reslut['left']['blob_flag']:
            is_turn_left = True
        if reslut['right']['blob_flag']:
            is_turn_right = True


    # �ж��Ƿ�ΪT�εĹ��
    is_t = False
    # �ж��Ƿ�ʮ���ι��
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

    # cx_mean ����ȷ���ӽ��еĹ������
    # ���ڱ�ʾ����ƫ����
    cx_mean = 0
    
    for roi_direct in ['up', 'down', 'middle']:
        if reslut[roi_direct]['blob_flag']:
            cx_mean += reslut[roi_direct]['cx']
        else:
            cx_mean += IMG_WIDTH / 2
    cx_mean /= 3

    # cx, cy ֻ����T�������������ʱ������ã�
    # ����ȷ�������Բ�εĴ������� ���ڶ��㣬 �Ǽ���Բ�ĵ�һ�ֽ��Ʒ���
    # cx, cy ֻ����T�������������ʱ������ã�
    # ����ȷ�������Բ�εĴ������� ���ڶ��㣬 �Ǽ���Բ�ĵ�һ�ֽ��Ʒ���
    cx = 0
    cy = 0

    if is_cross or is_t:
        # ֻ�ڳ���ʮ���λ���T���βż���Բ������
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
