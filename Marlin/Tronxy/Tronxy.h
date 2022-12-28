
#pragma once

#include "board/yBoard.h"

extern volatile bool serialPause;
#if TRONXY_UI
#define	STM32_IDADDR        STM32F4_IDADDR
#define MCU_NAME            "STM32F4XX"
#define MANUFACTURATOR      "TRONXY"
#define TRONXY_SOFT_VERSION  "V2.17.0"

#include "lcd/picdata/ySca_s.h"
#include "lcd/picdata/yBin_s.h"
#include "lcd/picdata/yPic_s.h"

#include "1romdata/romdata.h"
#include "0windows/yIcon.hpp"
#include "0windows/yFigure.hpp"
#include "0windows/yMultiple.hpp"
#include "0windows/yTablet.hpp"
#include "0windows/yVariant.hpp"
#include "0windows/yCircleButton.hpp"
#include "0windows/yWindow.hpp"
#include "0windows/yCompress.h"
#include "ySelf.h"

extern volatile bool globalStop;
extern yLabel *titleInfo;
void createTitleInfo(void);
void tronxyStop(void(*after)(void));
void tronxyStopAfter(void);
extern void (*stopAfter)(void);
void stopEmergency(void);

void tronxyInit(void);
void tronxyUpdate(void);
void tronxy_config_init(void);
void filamentSensorReset(void);
void screenPopIndex(int index,VoidFun other = nullptr,bool recordLastScreen = true);
void screenCloseMessageBox(void);
void screenFilament(void);
void screenKillError(const char*errorInfo1, const char* errorInfo2);
void screenMain(void);
void gobackScreen(VoidFun screen);
void turnToTheme(uint8_t theme);
void screenPopInfo(const char*info);
void screenReadFailAfter(void);
void saveSettingInPrinting(void);
void pauseMotion(void);
void resumeMotion(void);
void moveEAxis(uint8_t e,bool dir);
void keepEMove(void);
void screenFactoryReset(void);
void logScreen(void);
bool showLog(void);
void reshowWindow(void);
void autoLevelEnd(void);
extern uint8_t my_lang,my_theme,buzzer_switch;
extern uint8_t my_test,myui_ready;

extern volatile uint8_t model_light_st;
extern uint8_t extruder_num,
	hotend_num,
	out_extruder,//记录断料的挤出机
	throat_fan,//为0表示不使用喉管风扇,其它值代表喉管风扇在风扇列表中的起始位置
	fifth_mode,
	dir_y_axis,//影响y轴图标形为,如果图标点击后移动方向是反的,则倒转此值
	dir_z_axis,//影响热床Z上下图标形为,如果图标点击后移动方向是反的,则倒转此值
	xy_device_id;

extern uint8_t pin_left_z_endstop, 
	pin_right_z_endstop,
	pin_probe,
	pin_e0_filament,
	pin_e1_filament,
	pin_e2_filament,
	pin_x_stop,
	pin_y_stop,
	pin_x2_stop;

extern volatile uint8_t enabled_laser_serial,
	enabled_auto_bed_leveling,
	enabled_filament_sensor,
	enabled_controller_fan,
	enabled_dual_x,
	enabled_dual_z_stop,
	enabled_z_max_home,
	enabled_taoli,
	enabled_need_heat_head,
	enabled_need_heat_bed,
	enabled_fan,
	enabled_light, //使能照明灯,用主板风扇口,必须失能USE_CONTROLLER_FAN
	enabled_only_home_xy,
	enabled_usb;

extern volatile uint8_t auto_leveling_x_points, auto_leveling_y_points;
extern void (*updateAutoLevel)(int x, int y, float* value);

extern float fifth_e_factor;
extern float pos_x_min,
	pos_x_max,
	pos_y_min,
	pos_y_max,
	pos_z_min,
	pos_z_max,
	pos_x2_min,
	pos_x2_max,
	pause_z_raise,
	auto_leveling_max_err;

extern yWindow *topwin;
extern VoidFun myCurrentFunction;
extern int messageIndexNow,messageIndexNext;
extern int touch_x,touch_y;
extern uint32_t logTime,printTotalTime;
extern const char *screen_error1, *screen_error2;
extern xyz_pos_t Probe_Offset;

extern const char* software_version;
extern const char* firmware_name;

#define POP_FILAMENT_OUT              1
#define POP_PAUSING                   2
#define POP_POWERLOSS                 3
#define POP_WAITNOZZLE_HEAT           4
#define POP_KILL_ERROR                5 
#define POP_LANGUAGE                  6
#define POP_FACTORY_RESET             7
#define POP_CALIBRATE_CONFIRM         8
#define POP_STOP_PRINT                9
#define POP_COMPLETE_PRINT            10
#define POP_DELETE_FILE               11
#define POP_EXTRUDER_COLD             12
#define POP_FILAMENT_INSERT           13
#define POP_RECOVERY_PRINT            14
#define POP_NOSUPPORT                 15
#define POP_PROB_FAIL                 16
#define POP_NORMAL_MODE               17
#define POP_SAVE_HOTENDOFFSET         18
#define POP_OPENFILE_FAIL             19
#define POP_READFILE_FAIL             20
#define POP_INSERT_CARD               21
#define POP_SINGLE_INFO               22
#define POP_RESTART_TO_CALI           23
#define POP_SAVE_OK                   24
#define POP_LEVEL_OK                  25
#define POP_LEVEL_NG                  26
#define POP_DUALZ_LIMIT_ERR           27

#if USBHOST_HS_EN
#define NAME_SIGN_USB        "U"
#else
#define NAME_SIGN_USB        "0"
#endif
#if USE_EXTRUDER == 1
#define NAME_SIGN_EXTRUDER   "T"
#elif USE_EXTRUDER == 2
#define NAME_SIGN_EXTRUDER   "B"
#elif USE_EXTRUDER == 3
#define NAME_SIGN_EXTRUDER   "t"
#elif USE_EXTRUDER == 4
#define NAME_SIGN_EXTRUDER   "g"
#else
#error "wrong extruder!"
#endif

#define SHUTDOWN_PERIOD        90000

#define LANG_ENGLISH      0
#define LANG_CHINESE      1
#define LANG_JAPANESE     2
#define LANG_KOREAN       3
#define LANG_RUSSIAN      4
#define LANG_FRENCH       5
#define LANG_GERMAN       6
#define LANG_SPANISH      7
#define LANG_PORTUGUESE   8
enum TronxyPrintStatus
{
	PRINT_IDLE = 0,
	PRINT_AUTO_LEVEL,//在自动调平中
	PRINT_ERROR,

	PRINT_RUNNING,//打印中
	PRINT_RUNNING_HOME,//打印中复位
	PRINT_PAUSE,//暂停
	PRINT_RESUME,//恢复中
	PRINT_ABORTING,//打印退出中

	PRINT_RUNNING_SERIAL,//连机打印
	PRINT_PAUSE_SERIAL,
};
extern volatile TronxyPrintStatus my_print_status;

#define INDEX_LANG          104
#define INDEX_THEME         (INDEX_LANG + 1)
#define INDEX_BUZZER_SWITCH (INDEX_THEME + 1)
#define INDEX_DUAL_X_MODE   (INDEX_BUZZER_SWITCH + 1)
#define INDEX_CALIBRATION   (INDEX_DUAL_X_MODE + 1)
#define INDEX_FIFTH_EFACTOR (INDEX_CALIBRATION + 12)
#define INDEX_BABYSTEP_ACCUM (INDEX_FIFTH_EFACTOR + 4)
#define INDEX_Z_OFFSET      (INDEX_BABYSTEP_ACCUM + 2)
#define INDEX_HOTEND_OFFSET (INDEX_Z_OFFSET + sizeof(Probe_Offset))
#define INDEX_Z_VALUES      (INDEX_HOTEND_OFFSET + sizeof(xyz_pos_t) + 2)
constexpr int indexzvalue = INDEX_Z_VALUES;

#if HOTENDS && HOTENDS > 2
#error settings parameter position has changed.
#endif

#endif

typedef struct
{
  uint32_t SYSCLK_Frequency;  /*!< returns SYSCLK clock frequency expressed in Hz */
  uint32_t HCLK_Frequency;    /*!< returns HCLK clock frequency expressed in Hz */
  uint32_t PCLK1_Frequency;   /*!< returns PCLK1 clock frequency expressed in Hz */
  uint32_t PCLK2_Frequency;   /*!< returns PCLK2 clock frequency expressed in Hz */
  uint32_t USB_Frequency;
  uint32_t SDIO_Frequency;
}RCC_ClocksTypeDef;
void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks);

#define TEST_NO_PRINT     0
