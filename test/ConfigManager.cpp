/*
 * display/ConfigManager.cpp
 */
#include "fw3d/display/ConfigManager.h"
#include "fw3d/display/ImageAreaManage.h"
#include "fw3d/display/ImageArea.h"
#include "fw3d/display/StateDispatch.h"
#include "fw3d/display/PrepareCtrl.h"
#include "fw3d/display/StateProxy.h"
//#include "fw3d/display/ReplayManage.h"
#include "fw3d/display/slices/SlicesArea.h"

#include "fw3d/ThreadKit.h"
#include "fw3d/Render/UIS4DRender.h"
#include "fw3d/UIS4D/UIS4D.h"
#include "fw3d/display/Colors.h"

//namespace {
    gboolean ClearRotationHint(gpointer data)
    {
        ImageArea::GetInstance()->RotationHint("- X -", true);
        ImageArea::GetInstance()->RotationHint("- Y -", true);
        ImageArea::GetInstance()->RotationHint("- Z -", true);
        return false;
    }

    void ScanDistance(double value)
    {
		
        CThreadKit::GetInstance()->GetWriteLock();
        CUIS4D::GetInstance()->SetScanDistance((int)value);
        CThreadKit::GetInstance()->ReadWriteUnlock();
		
    }

    void FHSwingAngle(double value)
    {
		
        float value_real = value*SWING_ANGLE_STEP/SWING_ANGLE_STEP_DIV;

        CThreadKit::GetInstance()->GetWriteLock();
        CUIS4D::GetInstance()->SetSwingAngle(value_real);
        CThreadKit::GetInstance()->ReadWriteUnlock();
		
    }

    void VPSwingAngle(double value)
    {
		
		float value_real = 0.0;
        value_real = value*SWING_ANGLE_STEP/SWING_ANGLE_STEP_DIV;

        if (static_cast<int>(value_real)%10 < 5)
        {
            value_real = value_real-static_cast<int>(value_real) % 10;
        }
        else
        {
            value_real = (static_cast<int>(value_real/10) + 1) * 10;
        }

        CThreadKit::GetInstance()->GetWriteLock();
        CUIS4D::GetInstance()->SetSwingAngle(value_real);
        CThreadKit::GetInstance()->ReadWriteUnlock();

        if (!CUIS4D::GetInstance()->GetD3Flag())  //4D
        {
//            ConfigManager::GetInstance()->SetPrepareVPSwingAngle3D(value); 
//            ConfigManager::GetInstance()->ChangeVPQuality(value);
            CThreadKit::GetInstance()->GetWriteLock();
            CUIS4D::GetInstance()->SetQualityLevel((unsigned char)value);
            CThreadKit::GetInstance()->ReadWriteUnlock();
        }
        else
        {
//            ConfigManager::GetInstance()->SetPrepareVPSwingAngle4D(value);
            CThreadKit::GetInstance()->GetWriteLock();
            CUIS4D::GetInstance()->SetQualityLevel(
                ConfigManager::GetInstance()->SysConf()["Prepare.SwingAngle"].DefaultValue);
            CThreadKit::GetInstance()->ReadWriteUnlock();
        }
		
    }

    void VPQuality(double value)
    {
		
//        ConfigManager::GetInstance()->SetPrepareVPQuality4D(value); 
        CThreadKit::GetInstance()->GetWriteLock();
        CUIS4D::GetInstance()->SetQualityLevel((unsigned char)value);
        CThreadKit::GetInstance()->ReadWriteUnlock();
		
    }

    void Render(double value)
    {
		
        if (DISP_FORMAT_11 == StateProxy::GetInstance()->DispFormatType())
        {
            if (StateDispatch::VIEW == StateDispatch::GetInstance()->GetWorkState())
            {    
                StateProxy::GetInstance()->GetView3DCtrl()->emit_ref_button_clicked(REF_IMAGE_3D);
            }
            else
            {
                StateProxy::GetInstance()->GetCapture4DCtrl()->emit_ref_button_clicked(REF_IMAGE_3D);
            }
        }

        CThreadKit::GetInstance()->GetWriteLock();

        CUIS4DRender::GetInstance()->SetRenderMode(RENDERMAXIP+(int)value);

        CUIS4D::GetInstance()->SetModifyFlag(true);
        CThreadKit::GetInstance()->ReadWriteUnlock();
		
    }
    
    void Vision(double value)
    {
		
//        CThreadKit::GetInstance()->GetWriteLock();            
//        CUIS4DRender::GetInstance()->SetVision((int)value);
//        CUIS4D::GetInstance()->SetModifyFlag(true);
//        CThreadKit::GetInstance()->ReadWriteUnlock();

        ConfigManager::GetInstance()->SetVisionGroup(value);

//        CThreadKit::GetInstance()->GetWriteLock();            
//        CUIS4DRender::GetInstance()->SetDensity(value);
//        CUIS4DRender::GetInstance()->SetBrightness(value);
//        CUIS4DRender::GetInstance()->SetContrast(value);
//        CUIS4D::GetInstance()->SetSmooth(value);
//
//        CUIS4D::GetInstance()->SetModifyFlag(true);
//        CThreadKit::GetInstance()->ReadWriteUnlock();
		
    }

    void QRotate(double value)
    {
		
        CThreadKit::GetInstance()->GetWriteLock();            
        CUIS4DRender::GetInstance()->SetQRotate((float)value);
        CUIS4D::GetInstance()->SetModifyFlag(true);
        CThreadKit::GetInstance()->ReadWriteUnlock();
		
    }

    void Chroma(double value)
    {
		
        switch (StateDispatch::GetInstance()->GetWorkState())
        {
            case StateDispatch::VIEW:
            {
                StateProxy::GetInstance()->GetView3DCtrl()->SetChroma(value);
                break;
            }
            case StateDispatch::CAPTURE_4D:
            {
                StateProxy::GetInstance()->GetCapture4DCtrl()->SetChroma(value);
                break;
            }
            default:;
        }

		
    }

    void Graymap(double value)
    {
		
        CThreadKit::GetInstance()->GetWriteLock();
        CUIS4DRender::GetInstance()->SetGraymap((int)value);
        CUIS4D::GetInstance()->SetModifyFlag(true);
        CThreadKit::GetInstance()->ReadWriteUnlock();

        RGBTRIPLE *color = CUIS4DRender::GetInstance()->GetColormap();
        RGBTRIPLE *gray = ImageAreaManage::GetInstance()->RenderTranslateColor(color, value);
        ImageAreaManage::GetInstance()->SetGrayTable(gray);
        ImageAreaManage::GetInstance()->UpdateGrayScale();

		
    }

    void RotationX(double value)
    {
		
        ImageAreaManage::GetInstance()->UpdateQuarterImage(true, false, false);
        ImageAreaManage::GetInstance()->UpdateTwoImage(true, false, false);
        ImageAreaManage::GetInstance()->UpdateOneImage(true, false, false, false);

        std::string rot("- X -");
        ImageArea::GetInstance()->RotationHint(rot);
        g_timeout_add(2, ClearRotationHint, 0);

        CThreadKit::GetInstance()->GetWriteLock();
        CUIS4DRender::GetInstance()->SetRotationX((float)value);
        CUIS4D::GetInstance()->SetModifyFlag(true);
        CThreadKit::GetInstance()->ReadWriteUnlock();
		
    }

    void RotationY(double value)
    {
		
        ImageAreaManage::GetInstance()->UpdateQuarterImage(false, true, false);
        ImageAreaManage::GetInstance()->UpdateTwoImage(false, true, false);
        ImageAreaManage::GetInstance()->UpdateOneImage(false, true, false, false);

        std::string rot("- Y -");
        ImageArea::GetInstance()-> RotationHint(rot);
        g_timeout_add(2, ClearRotationHint, 0);

        CThreadKit::GetInstance()->GetWriteLock();
        CUIS4DRender::GetInstance()->SetRotationY((float)value);
        CUIS4D::GetInstance()->SetModifyFlag(true);
        CThreadKit::GetInstance()->ReadWriteUnlock();

		
    }

    void RotationZ(double value)
    {
		
        ImageAreaManage::GetInstance()->UpdateQuarterImage(false, false, true);
        ImageAreaManage::GetInstance()->UpdateTwoImage(false, false, true);
        ImageAreaManage::GetInstance()->UpdateOneImage(false, false, true, false);

        std::string rot("- Z -");
        ImageArea::GetInstance()-> RotationHint(rot);
        g_timeout_add(2, ClearRotationHint, 0);

        CThreadKit::GetInstance()->GetWriteLock();
        CUIS4DRender::GetInstance()->SetRotationZ((float)value);
        CUIS4D::GetInstance()->SetModifyFlag(true);
        CThreadKit::GetInstance()->ReadWriteUnlock();

		
    }

    void Zoom(double value)
    {
		
        ImageArea::GetInstance()->Magnify(value);///50);
        ImageAreaManage::GetInstance()->UpdateQuarterImage(false, false, false);
        ImageAreaManage::GetInstance()->UpdateTwoImage(false, false, false);
        ImageAreaManage::GetInstance()->UpdateOneImage(false, false, false);

        CThreadKit::GetInstance()->GetWriteLock();  
        CUIS4DRender::GetInstance()->SetZoom((float)value);
        CUIS4D::GetInstance()->SetModifyFlag(true);
        CThreadKit::GetInstance()->ReadWriteUnlock();
		
    }

    void PlacentaThreshold(double value)
    {
		
        CThreadKit::GetInstance()->GetWriteLock();  
        CUIS4D::GetInstance()->SetPlacentaThresh((int)value);
        CUIS4D::GetInstance()->SetModifyFlag(true);
        CThreadKit::GetInstance()->ReadWriteUnlock();
		
    }

    void ChangeShift(double value)
    {
		
        SlicesArea::GetInstance()->SetShiftValue(value);
        CThreadKit::GetInstance()->GetWriteLock();
        CUIS4D::GetInstance()->SetMultiSliceFlag(true);
        CThreadKit::GetInstance()->ReadWriteUnlock();
		
    }

    void Density(double value)
    {
		
        CThreadKit::GetInstance()->GetWriteLock();
        CUIS4DRender::GetInstance()->SetDensity(value);
        CUIS4D::GetInstance()->SetModifyFlag(true);
        CThreadKit::GetInstance()->ReadWriteUnlock();
		
    }

    void Threshold(double value)
    {
		
        CThreadKit::GetInstance()->GetWriteLock();
        CUIS4DRender::GetInstance()->SetThreshold(value);
        CUIS4D::GetInstance()->SetModifyFlag(true);
        CThreadKit::GetInstance()->ReadWriteUnlock();
		
    }

    void Brightness(double value)
    {
		
        CThreadKit::GetInstance()->GetWriteLock();
        CUIS4DRender::GetInstance()->SetBrightness(value);
        CUIS4D::GetInstance()->SetModifyFlag(true);
        CThreadKit::GetInstance()->ReadWriteUnlock();
		
    }

    void Contrast(double value)
    {
		
        CThreadKit::GetInstance()->GetWriteLock();
        CUIS4DRender::GetInstance()->SetContrast(value);
        CUIS4D::GetInstance()->SetModifyFlag(true);
        CThreadKit::GetInstance()->ReadWriteUnlock();
		
    }

    void Smooth(double value)
    {
        CThreadKit::GetInstance()->GetWriteLock();
        CUIS4D::GetInstance()->SetSmooth(value);
        CUIS4D::GetInstance()->SetModifyFlag(true);
        CThreadKit::GetInstance()->ReadWriteUnlock();
    }
//}

ConfigManager* ConfigManager::ptr_instance_ = NULL;

ConfigManager* ConfigManager::GetInstance()
{
    if ( ptr_instance_ == NULL)
        ptr_instance_ = new ConfigManager;
    return ptr_instance_;
}

ConfigManager::ConfigManager()
{
    mode_ = KNOB_LINEAR;

//    InitConfig();
}

void ConfigManager::GetViewConfig(std::vector<double> &view_value)
{
    view_value.clear();
    
    for (std::vector<KeyCallback>::iterator it = FnMap[KNOB_VIEW].begin();
        it != FnMap[KNOB_VIEW].end(); it++)
    {
        view_value.push_back(REAL_VAL(user_conf_[it->key_].Value));
    }
}

ConfigManager::~ConfigManager()
{
    MEMBER_RELEASE(ptr_instance_)
}

void ConfigManager::InitConfig()
{
    try
    {
        SysConfLite u = SysConfLite(CONF_DB_FILE);

        u.ConfTable("SysConf");
        sys_conf_ = u.get_all();
    }
    catch (std::exception &e)
    {
        LOG_ERR(e.what())
        throw std::runtime_error("ConfigManager: Failed to read system configure");
    }

    try
    {
        UserConfLite u = UserConfLite(CONF_DB_FILE);

        u.ConfTable("UserConf");
        user_conf_ = u.get_all();
    }
    catch (std::exception &e)
    {
        LOG_ERR(e.what())
        throw std::runtime_error("ConfigManager: Failed to read user configure");
    }

    FnMap[KNOB_LINEAR].push_back(KeyCallback("Prepare.Distance", ScanDistance));
    FnMap[KNOB_FAN].push_back(KeyCallback("Prepare.SwingAngle", FHSwingAngle));

    FnMap[KNOB_VP_4D].push_back(KeyCallback("Prepare.SwingAngle", VPSwingAngle));
    FnMap[KNOB_VP_4D].push_back(KeyCallback("Prepare.QualityIndex", VPQuality));

    FnMap[KNOB_VP_3D].push_back(KeyCallback("Prepare.SwingAngle", VPSwingAngle));
    FnMap[KNOB_CAPTURE] = std::vector<KeyCallback>();

    FnMap[KNOB_VIEW].push_back(KeyCallback("View.Vision", Vision));
    FnMap[KNOB_VIEW].push_back(KeyCallback("View.Chroma", Chroma));
    FnMap[KNOB_VIEW].push_back(KeyCallback("View.Render", Render));
    FnMap[KNOB_VIEW].push_back(KeyCallback("View.Threshold", PlacentaThreshold));
    FnMap[KNOB_VIEW].push_back(KeyCallback("", 0));
    FnMap[KNOB_VIEW].push_back(KeyCallback("View.QRotate", QRotate));
    FnMap[KNOB_VIEW].push_back(KeyCallback("View.Zoom", Zoom));
    FnMap[KNOB_VIEW].push_back(KeyCallback("View.RotationX", RotationX));
    FnMap[KNOB_VIEW].push_back(KeyCallback("View.RotationY", RotationY));
    FnMap[KNOB_VIEW].push_back(KeyCallback("View.RotationZ", RotationZ));

    FnMap[KNOB_REVIEW] = std::vector<KeyCallback>();
    FnMap[KNOB_SLICES].push_back(KeyCallback("Slice.Shift", ChangeShift));
    FnMap[KNOB_DEBUG].push_back(KeyCallback("Debug.Density", Density));
    FnMap[KNOB_DEBUG].push_back(KeyCallback("Debug.Brightness", Brightness));
    FnMap[KNOB_DEBUG].push_back(KeyCallback("Debug.Contrast", Contrast));
    FnMap[KNOB_DEBUG].push_back(KeyCallback("Debug.Smooth", Smooth));
    FnMap[KNOB_DEBUG].push_back(KeyCallback("View.Vision", Vision)); // share the same item with VIEW
}

void ConfigManager::KnobF1(bool oper)
{
    SetKnobValue(0, oper);        
}

void ConfigManager::KnobF2(bool oper)
{
    SetKnobValue(1, oper);  
}

void ConfigManager::KnobF3(bool oper)
{
    SetKnobValue(2, oper); 
}

void ConfigManager::KnobF4(bool oper)
{
    SetKnobValue(3, oper); 
}

void ConfigManager::KnobF5(bool oper)
{
    SetKnobValue(4, oper); 
}

std::string ConfigManager::GetItemName(std::string key)
{
    size_t a = key.find_first_of(".");

    if (a == std::string::npos)
        return "";

    size_t b = key.find_first_of(".", ++a);

    if (b == std::string::npos) 
        b = key.size();

    return key.substr(a, (b-a));
}

void ConfigManager::UpdateParamLineEach(std::string key, int index_key)
{
    try
    {
        std::string val = TranslateKnobValue(key, REAL_VAL(user_conf_[key].Value));

        if (val.empty())
        {
           val = user_conf_[key].Value;
        }
        
        if (!sys_conf_[key].Unit.empty())
            val += sys_conf_[key].Unit;

        LOG(" " << index_key << " => " << key << ": " << val)
        SetParamLine(index_key, GetItemName(key), val);
    }
    catch (std::exception &e)
    {
        LOG_ERR(e.what())
        throw std::runtime_error(
        "ConfigManager: Failed to update parameters line");
    }
}

void ConfigManager::SetKnobValue(int index_key, int oper, double value)
{
    try
    {
        if (index_key >= (int)FnMap[mode_].size())
        {
            LOG_ERR("ConfigManager: "
                << " Index error index_key=" << index_key);
            return;
        }

        std::string key = FnMap[mode_][index_key].key_;

        if (key.empty())
        {
            LOG_ERR("ConfigManager: " << " Empty key error");
            return;
        }
        
        DEBUG_KEY(key);

        DoSetKnobValue(key, oper, value);

        if (user_conf_.find(key) != user_conf_.end()
            && FnMap[mode_][index_key].callback_ != 0)
        {
    		double val = REAL_VAL(user_conf_[key].Value);
	        (FnMap[mode_][index_key].callback_)(val);
        }

        UpdateParamLineEach(key, index_key);
    }
    catch (std::exception &e)
    {
        LOG_ERR(e.what())
        throw std::runtime_error("ConfigManager: Failed to set values");
    }
}

void ConfigManager::SetParamLine(
    int index_key, std::string name, std::string value)
{
    if (name.empty())
    {
        LOG("ConfigManager: " << "Empty key for item")
        return;
    }

    if (index_key < IMAGE_PARAM_NR)
    {
        std::ostringstream o;

        o << "<F" << (index_key+1) << "> " << _(name.c_str()) << ": " << value;
        gtk_button_set_label(GTK_BUTTON(image_param_[index_key]), 
            _(o.str().c_str()));
    }
    else if (index_key < IMAGE_PARAM_NR+IMAGE_PARAM_UPPER_NR)
    {
        std::ostringstream o;
        o << _(name.c_str()) << ": " << value;

        gtk_label_set_text(
            GTK_LABEL(image_param_upper_[index_key-IMAGE_PARAM_NR]),
            _(o.str().c_str()));
    }
    else
    {
        LOG_ERR("ConfigManager: "
            << "Index error index_key=" << index_key);
    }
}

void ConfigManager::ClearParamLine()
{
    int index = 0;

    while (index < IMAGE_PARAM_NR)
    {
        gtk_button_set_label(GTK_BUTTON(image_param_[index++]), " ");
    }

    while (index < IMAGE_PARAM_NR+IMAGE_PARAM_UPPER_NR)
    {
        gtk_label_set_text(GTK_LABEL(image_param_upper_[index-IMAGE_PARAM_NR]), " ");
        index++;
    }

    for (size_t i = 0; i < FnMap[mode_].size(); i++)
    {
        std::string key = FnMap[mode_][i].key_;

        DEBUG_KEY(key);

        SetParamLine(i, GetItemName(key),
            user_conf_[key].Value+sys_conf_[key].Unit);
    }
}

void ConfigManager::DoSetKnobValue(
    std::string key, int oper, double real_value)
{
    try {

        double step = sys_conf_[key].Step;
        double upper = sys_conf_[key].Upper;
        double lower = sys_conf_[key].Lower;
        double value = REAL_VAL(user_conf_[key].Value);
    
        UserConfLite u = UserConfLite(CONF_DB_FILE);
        u.ConfTable("UserConf");
    
        switch (oper)
        {
            case -1:
            {
                if (real_value <= upper && real_value >= lower)
                    user_conf_[key].Value = encode_single<double>(real_value);
                break;
            }
            case 0:
            {
                if ((value-step-lower) < 0)
                    value = upper;
                else
                    value -= step;
    
                   user_conf_[key].Value = encode_single<double>(value);
                break;
            }
            default:
            {
                if ((value + step - upper) > 0)
                    value = lower;
                else
                    value += step;
                   user_conf_[key].Value = encode_single<double>(value);
                break;
            }
        }
        
        /* update persist configure */
        if (key == "View.Zoom" 
		&& StateProxy::GetInstance()->DispFormatType() == DISP_FORMAT_22)
		{
            u.set_value(key, value*2.0);
		}
        else if (key == "View.RotationX" || key == "View.RotationY"
			|| key == "View.RotationZ")
		{
			return;
		}
		else
		{
            u.set_value(key, value);
		}
    }
    catch (std::exception &e)
    {
        LOG_ERR(e.what())
    }
}

void ConfigManager::CreateImageParam(GtkWidget* fixed)
{
    GtkWidget* line1 = gtk_hbox_new(true, 3);
    GtkWidget* line2 = gtk_hbox_new(true, 0);

    gtk_fixed_put(GTK_FIXED(fixed), line1, UIS4DFACE::IMAGE_PARAM_X, UIS4DFACE::IMAGE_PARAM_Y);
    gtk_widget_set_size_request(line1, UIS4DFACE::IMAGE_PARAM_WIDTH, UIS4DFACE::IMAGE_PARAM_HEIGHT);

    gtk_fixed_put(GTK_FIXED(fixed), line2, UIS4DFACE::IMAGE_START_X, UIS4DFACE::HINT_Y-UIS4DFACE::IMAGE_PARAM_HEIGHT/2-5);
    gtk_widget_set_size_request(line2, UIS4DFACE::IMAGE_WIDTH/2, UIS4DFACE::IMAGE_PARAM_HEIGHT/2);

    for (int i = 0; i < IMAGE_PARAM_NR; i++)
    {
        image_param_[i] = gtk_button_new();
        gtk_box_pack_start (GTK_BOX (line1), image_param_[i], TRUE, TRUE, 0);
//        gtk_misc_set_alignment(GTK_MISC(image_param_[i]), 0, 0);
        gtk_button_set_alignment(GTK_BUTTON(image_param_[i]), 0.5, 0.5);
    }

    for (int i = 0; i < IMAGE_PARAM_UPPER_NR; i++)
    {
        image_param_upper_[i] = gtk_label_new(" ");
        gtk_box_pack_start(GTK_BOX (line2), image_param_upper_[i], TRUE, TRUE, 0);
        gtk_misc_set_alignment(GTK_MISC(image_param_upper_[i]), 0, 0);
    }

    gtk_widget_show(line1);
    gtk_widget_show(line2);

}

void ConfigManager::SwitchFocus(bool param0)
{
    int index = 0;

    while (index < IMAGE_PARAM_NR)
        gtk_widget_set_sensitive(image_param_[index++], param0);

    ClearParamLine();
}

void ConfigManager::UpdateParamLine()
{
    for (size_t i = 0; i < FnMap[mode_].size(); i++)
      {
        std::string key = FnMap[mode_][i].key_;

        if (key.empty())
            continue;

        UpdateParamLineEach(key, i);
    }
}

void ConfigManager::UpdateValues()
{
    for (size_t i = 0; i < FnMap[mode_].size(); i++)
      {
        std::string key = FnMap[mode_][i].key_;

        if (key.empty())
            continue;

        DEBUG_KEY(key);

        double value = REAL_VAL(user_conf_[key].Value);
        SetKnobValue(i, -1, value);
    }
}

void ConfigManager::SendViewValues()
{
    CUIS4DRender::GetInstance()->ResetMatrix();
    CUIS4DRender::GetInstance()->ResetMask();

    size_t qr_id = 0;

    for (std::vector<KeyCallback>::iterator it = FnMap[KNOB_VIEW].begin();
        it != FnMap[KNOB_VIEW].end(); it++)
    {
        if (it->key_ == "View.RotationX"
        || it->key_ == "View.RotationY"
        || it->key_ == "View.RotationZ")
            continue;

        if (it->key_ == "View.QRotate")
        {
            qr_id = std::distance(FnMap[KNOB_VIEW].begin(), it);
            continue;
        }

        if (user_conf_.find(it->key_) != user_conf_.end()
            && it->callback_ != 0 && !it->key_.empty())
        {
    		double val = REAL_VAL(user_conf_[it->key_].Value);
            (it->callback_)(val);
        }
    }

    CUIS4DRender::GetInstance()->ResetXYZ(
        REAL_VAL(user_conf_["View.RotationX"].Value),
        REAL_VAL(user_conf_["View.RotationY"].Value),
        REAL_VAL(user_conf_["View.RotationZ"].Value)
    );

    if (qr_id < FnMap[KNOB_VIEW].size())
	{
    	double val = REAL_VAL(user_conf_["View.QRotate"].Value);
        (FnMap[KNOB_VIEW][qr_id].callback_)(val);
	}
}

void ConfigManager::ResetView()
{
    CUIS4DRender::GetInstance()->ResetMatrix();
    CUIS4DRender::GetInstance()->ResetMask();

    UserConfLite u = UserConfLite(CONF_DB_FILE);
    u.ConfTable("UserConf");
    u.reset_by_prefix("View.");
//    u.reset_by_prefix("Debug."); // TO BE CONFIRM ???
    user_conf_ = u.get_all();

    SendViewValues();
    UpdateParamLine();

	if (StateProxy::GetInstance()->DispFormatType() == DISP_FORMAT_22)
		zoom_in_22();
}

void ConfigManager::PrepareMode()
{
    bool is_vp = StateProxy::GetInstance()->GetProbeVP();
    PrepareCtrl::ScanType scan_type = StateProxy::GetInstance()->GetScanType();

    if (is_vp) //probe vp
    {
        if (scan_type == PrepareCtrl::S_LINEAR_4D)
            mode_ = KNOB_VP_4D;
        else
            mode_ = KNOB_VP_3D;
    }
    else
    {
        if (scan_type == PrepareCtrl::S_LINEAR_4D)
        {
            mode_ = KNOB_LINEAR;
        }
        else// if (scan_type == PrepareCtrl::S_FAN_3D)
        {
            mode_ = KNOB_FAN;
        }
    }

    SwitchFocus(true);
    UpdateValues();
}

void ConfigManager::Capture4DMode()
{
//    mode_ = KNOB_4DCAPTURE;
    mode_ = KNOB_VIEW;
    SwitchFocus(true);
    SendViewValues();
    UpdateParamLine();
}

void ConfigManager::CaptureMode()
{
    mode_ = KNOB_CAPTURE;
    SwitchFocus(false);
}

void ConfigManager::Debug()
{
    if (KNOB_DEBUG != mode_)
    {
        mode_before_ = mode_;
        mode_ = KNOB_DEBUG;

        double value = decode_single<double>(
            user_conf_["View.Vision"].Value);
            
           SwitchFocus(true);
        SetVisionGroup(value);
    }
    else
    {
        mode_ = mode_before_;

           SwitchFocus(true);
        UpdateValues();
    }
}

void ConfigManager::ViewMode()
{
    mode_ = KNOB_VIEW;
    SwitchFocus(true);
}

void ConfigManager::ReviewMode()
{
    mode_ = KNOB_REVIEW;
    SwitchFocus(false);
}

void ConfigManager::SlicesMode()
{
    mode_ = KNOB_SLICES;
    SwitchFocus(true);
    UpdateValues();
}

std::string ConfigManager::TranslateKnobValue(std::string name, double value)
{
    std::string ret;

    if ("View.Render" == name)
    {
        value += RENDERMAXIP;
        switch ((int)value)
        {
            case RENDERMAXIP:
             ret += _("Max"); break;
            case RENDERMINIP:
             ret += _("Min"); break;
            case RENDERXRAY:
             ret += _("XRay"); break;
            case RENDERSURFACE:
             ret += _("Surface"); break;
            case RENDERLIGHT:
             ret += _("Light"); break;
            default: ret += "Invalid";
        }
    }
    else; // ...

    return ret;
}

void ConfigManager::zoom_in_22()
{
    LOG("zoom_in_22"); 
    zoom_special(SPEC_ZOOM_SMALLER);
}

void ConfigManager::zoom_in_12()
{
    LOG("zoom_in_12"); 
    zoom_special(SPEC_ZOOM_LARGER);
}

void ConfigManager::zoom_special(SpecZoom transfer)
{
    std::string key("View.Zoom");
    int index_key = -1;

    for (std::vector<KeyCallback>::iterator it = FnMap[KNOB_VIEW].begin();
        it != FnMap[KNOB_VIEW].end(); it++)
    {
        if (it->key_ == key)
        {
            index_key = std::distance(FnMap[KNOB_VIEW].begin(), it);
            break;
        }
    }

    if (0 > index_key)
    {
        LOG_ERR("No index for key " << key )
        return;
    }

    double val = REAL_VAL(user_conf_[key].Value);

    if (SPEC_ZOOM_LARGER == transfer)
    {
        sys_conf_[key].DefaultValue *= 2.0;
        sys_conf_[key].Step *= 2.0;
        sys_conf_[key].Upper *= 2.0;
        sys_conf_[key].Lower *= 2.0;
        val *= 2.0;
    }
    else
    {
        sys_conf_[key].DefaultValue /= 2.0;
        sys_conf_[key].Step /= 2.0;
        sys_conf_[key].Upper /= 2.0;
        sys_conf_[key].Lower /= 2.0;
        val /= 2.0;
    }

    user_conf_[key].Value = encode_single<double>(val);

    (FnMap[mode_][index_key].callback_)(val);
    UpdateParamLineEach(key, index_key);
}

void ConfigManager::SetQuality4D()
{
    CThreadKit::GetInstance()->GetWriteLock();
    CUIS4D::GetInstance()->SetD3Flag(false);
    CUIS4D::GetInstance()->SetQualityLevel(
        decode_single<unsigned char>(user_conf_["Prepare.Quality"].Value));
    CThreadKit::GetInstance()->ReadWriteUnlock();        
}

void ConfigManager::PreferredScan(std::string s)
{
    try
    {
        UserConfLite u = UserConfLite(CONF_DB_FILE);
        u.ConfTable("UserConf");

        user_conf_["Prepare.PreferredScan"].Value = s;
        u.set_value("Prepare.PreferredScan", s);
    }
    catch (std::exception &e)
    {
        LOG_ERR(e.what())
        throw std::runtime_error("ConfigManager: Failed to set preferred scan type");
    }
}

std::string ConfigManager::PreferredScan()
{
    if (user_conf_.find("Prepare.PreferredScan") != user_conf_.end())
        return user_conf_["Prepare.PreferredScan"].Value;
    LOG_ERR("ConfigManager: " << "read Prepare.PreferredScan failed")
    return "4D";
        
}

void ConfigManager::SetVisionGroup(double value)
{
//    CThreadKit::GetInstance()->GetWriteLock();            
//    CUIS4DRender::GetInstance()->SetVision((int)value);
//    CUIS4D::GetInstance()->SetModifyFlag(true);
//    CThreadKit::GetInstance()->ReadWriteUnlock();

    double vals[FnMap[KNOB_DEBUG].size()-1];

    for (size_t i = 0; i < FnMap[KNOB_DEBUG].size()-1; i++)
      {
        std::string key = FnMap[KNOB_DEBUG][i].key_;

        if (key.empty()) continue;

        key.append(".Vi");
		key += encode_single<double>(value).at(0);

        if (user_conf_.find(key) == user_conf_.end()) continue;

        vals[i] = REAL_VAL(user_conf_[key].Value);

        if (KNOB_DEBUG == mode_)
            UpdateParamLineEach(key, i);
    }

    if (sizeof(vals) < 4) return;

    CThreadKit::GetInstance()->GetWriteLock();            

    CUIS4DRender::GetInstance()->SetThreshold(3.0);
    CUIS4DRender::GetInstance()->SetDensity(vals[0]);
    CUIS4DRender::GetInstance()->SetBrightness(vals[1]);
    CUIS4DRender::GetInstance()->SetContrast(vals[2]);
    CUIS4D::GetInstance()->SetSmooth(vals[3]);

    CUIS4D::GetInstance()->SetModifyFlag(true);
    CThreadKit::GetInstance()->ReadWriteUnlock();
}
