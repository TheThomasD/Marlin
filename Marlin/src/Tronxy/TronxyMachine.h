#pragma once

#define TRONXY_MACHINE_NAME       "GeminiS"
#define TRONXY_BACKUP_POWER       0 //UPS功能,注意需要某些特定脚作断电检测,配置相应的中断函数
#define TRONXY_SCREEN_SHUTDOWN    1 //熄屏功能
#define TRONXY_HOME_XY_ONLY       0 //手动调试界面复位是否只有XY复位,如果是,则Z复位按钮激活,否则失效
#define TAOLI_SERIAL              0
#define LASER_SERIAL              0 //wifi端子用于激光PWM控制,这里可能会影响与octoprint的通信
#define DUAL_ZMAX_HOME            0 //大端复位,向反方向复位,再回到正方向找0点
#define Z_BED_MOVE                0 //Z轴移动:热床
#define USE_EXTRUDER              2 //T,B TITAN:1,BMG:2,TAOLI:3,general:4

