#!/usr/bin/env python
# encoding: GBK
import os
import re
import sys
import time
import datetime
import logging
import shutil
 
#reload(sys)
#sys.setdefaultencoding('utf-8')
 
logging.basicConfig(level=logging.DEBUG,
                    format='%(asctime)s %(filename)s[line:%(lineno)-1d] %(levelname)s %(message)s',
                    datefmt='%a, %d %b %Y %H:%M:%S',
                    filename='D:\\move.log',
                    filemode='a')
# logging.debug('This is debug message')
# logging.info('This is info message')
# logging.warning('This is warning message')
 
 
def find_file(file_dir, file_re='数据', expire_time=60):
    logging.info('传入参数 :目录 [%s]，正则表达式[%s],过期天数 [%s]' % (file_dir,file_re,expire_time))
    if not os.path.exists(file_dir):
        logging.info('传入参数 :目录 [%s]不存在' % file_dir)
        return None
         
#匹配文件或目录
 
    #目录下所有文件
    all_file = os.listdir(file_dir)
    #匹配正则的文件或目录
    reg_file_list = []
    reg_str = file_re
    for reg_file in all_file:
        #if os.path.isdir(reg_file):
        #    continue
        if re.match(reg_str,reg_file):
            logging.info('正则匹配到文件：[%s]' % reg_file)
            reg_file_list.append(reg_file)
    if len(reg_file_list) < 7:
        logging.info('匹配文件数小于7个，不进行移动操作！')
        return None
    #满足过期时间的文件
     
    #当前时间
    today = datetime.datetime.now()
     
    #1天前时间
    one_days = datetime.timedelta(days=1)
    one_days_agos = today - one_days
    #1天前时间文件夹
    one_days_agos_dir = one_days_agos.strftime("%Y-%m-%d")
    #1天前时间戳
    one_days_agos_timestamps = time.mktime(one_days_agos.timetuple())
     
    #n天前时间
    n_days = datetime.timedelta(days=int(expire_time))
    n_days_agos = today - n_days
    #n天前时间文件夹
    n_days_dir = n_days_agos.strftime("%Y-%m-%d")
    #n天前时间戳
    n_days_agos_timestamps = time.mktime(n_days_agos.timetuple())
 
    #新建目录000-00-00~0000-00-00
    date_dir = '%s_%s' %(n_days_dir,one_days_agos_dir)
    if not os.path.exists(os.path.join(file_dir,date_dir)):
        os.mkdir(os.path.join(file_dir,date_dir))
 
    #移动1~n天期间的文件或目录
    for date_file in reg_file_list:
        abs_file = os.path.join(file_dir,date_file)
        file_timestamp = os.path.getctime(abs_file)
        if float(n_days_agos_timestamps) <= float(file_timestamp) <= float(one_days_agos_timestamps):
            logging.info('移动文件：[%s]' % abs_file)
            #print "匹配到文件:" ,abs_file
            #移动满足条件的文件
            shutil.move(abs_file, os.path.join(file_dir,date_dir))
            logging.info('移动：[%s]到[%s]成功' % (abs_file,os.path.join(file_dir,date_dir))) 
 
def read_conf(file_path):
    with open(file_path,'r') as f:
        for line in f:
            line_list = line.strip().split(',')
            if len(line_list) != 3:
                logging.warning('%s 行配置不正确' % line.strip())
                continue
            file_dir = line_list[0]
            file_re= line_list[1]
            expire_time = line_list[2]
            find_file(file_dir,file_re,expire_time)
 
if __name__ == "__main__":
    read_conf(sys.argv[1])