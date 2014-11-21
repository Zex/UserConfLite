/* 
 * ConfigManager.h
 *
 * Author: Zex Li <top_zlynch@yahoo.com>
 *
 * +-----+ update sys config   +----------------+
 * | DES +---------<<CGI>>---->+                |
 * | /   |                     | config.db/sys  |
 * | ENG +----<<USB/TERM>>---->+                |
 * +-----+                     +--+-------------+
 *                                | reset with config.db/sys
 *                                V
 *     /-----+             +----------------+    customize  +------+
 *    | DEV  |   configure |                +<--<<IFACE>>---+      |
 *    |      |<------------| config.db/user |               | User |
 *     +--+--/             |                +<---<<IFACE>>--+      |
 *        |                +----------------+    reset op   +------+
 *        |                                                    A
 *        +----------------------------------------------------+
 *                         effect, response, image, blah
 */
#pragma once

#include <gtk/gtk.h>
#include <memory>
#include "fw3d/display/ConfLite.h"
#include <fstream>

enum KnobType {
	KNOB_LINEAR,
	KNOB_FAN,
	KNOB_VP_4D,
	KNOB_VP_3D,
	KNOB_CAPTURE,
	KNOB_VIEW,
	KNOB_REVIEW,
	KNOB_SLICES,
	KNOB_4DCAPTURE,
	KNOB_DEBUG
};

enum SpecZoom {	
	SPEC_ZOOM_SMALLER,
	SPEC_ZOOM_LARGER,
};

#define CONF_DB_FILE "/mnt/harddisk/emp/11002/res/config.db"
#define IMAGE_PARAM_NR 5
#define IMAGE_PARAM_UPPER_NR 2
#define REAL_VAL(v) decode_single<double>(v)
        
#define DEBUG_KEY(key)      \
    if (KNOB_DEBUG == mode_ && (key != "View.Vision"))	\
    {                       \
        key.append(".Vi");  \
        key += user_conf_["View.Vision"].Value.at(0);  \
    }


// mode => F1-key, F2-key ...
struct KeyCallback
{
	std::string key_;
	void (*callback_)(double value);

	KeyCallback() : callback_(0)
	{
	}

	KeyCallback(std::string k, void (*cb)(double))
	: key_(k), callback_(cb)
	{
	}
};

class ConfigManager
{
protected:

	void SwitchFocus(bool param0);

    ConfigManager();
	void DoSetKnobValue(std::string key, int oper, double real_value);
	void ClearParamLine();

    static ConfigManager* ptr_instance_;
	std::map<int, std::vector<KeyCallback>> FnMap;

	MAP_SC sys_conf_;
	MAP_UC user_conf_;

    int mode_, mode_before_;

	//zlynch
	GtkWidget* image_param_[IMAGE_PARAM_NR];
	GtkWidget* image_param_upper_[IMAGE_PARAM_UPPER_NR];

public:
    ~ConfigManager();
    static ConfigManager* GetInstance();
	void InitConfig();
	void UpdateValues();
	void UpdateParamLine();
	void UpdateParamLineEach(std::string key, int index_key);

	void SetParamLine(
		int index_key, std::string name, std::string value);
	std::string TranslateKnobValue(std::string name, double value);

	void CreateImageParam(GtkWidget* fixed);
	std::string GetItemName(std::string key);

    void PrepareMode();
    void CaptureMode();
    void Capture4DMode();
    void ViewMode();
	void Debug();
    void ReviewMode();
    void SlicesMode();
//  void SlicesDefault();

    void KnobF1(bool oper);
    void KnobF2(bool oper);
    void KnobF3(bool oper);
    void KnobF4(bool oper);
    void KnobF5(bool oper);

    void GetViewConfig(std::vector<double> &view_value);

	void ResetView();

    int Mode()
	{
		return mode_;
	}
    void SetKnobValue(int index_key, int oper, double real_value = 0.0);

    void SendViewValues();

	MAP_SC SysConf()
	{
		return sys_conf_;
	}

	MAP_UC UserConf()
	{
		return user_conf_;
	}

	void zoom_in_22();
	void zoom_in_12();
	void zoom_special(SpecZoom transfer);

	void SetVisionGroup(double value);
	void SetQuality4D();
	void PreferredScan(std::string s);
	std::string PreferredScan();
};

