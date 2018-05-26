#include <kodi/addon-instance/Visualization.h>
#include <kodi/General.h>

#include "nastyfft.h"

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

const std::vector<std::string> g_presets =
{
  "Standard",
  "Hero",
  "Flames"
};


class CVisualizationNastyFFT
  : public kodi::addon::CAddonBase,
    public kodi::addon::CInstanceVisualization
{
public:
  NastyFFT plugin;

  virtual ADDON_STATUS Create() override;
  virtual ADDON_STATUS GetStatus() override;
  virtual void GetInfo(bool& wantsFreq, int& syncDelay) override;
  virtual bool Start(int channels, int samplesPerSec, int bitsPerSample, std::string songName) override;
  virtual void Render() override;
  virtual void AudioData(const float* pAudioData, int iAudioDataLength, float *pFreqData, int iFreqDataLength) override;
  virtual ADDON_STATUS SetSetting(const std::string& settingName, const kodi::CSettingValue& settingValue) override;
  virtual bool GetPresets(std::vector<std::string>& presets) override;
  virtual bool LoadPreset(int select) override;
  virtual int GetActivePreset() override;
};

//-- Create -------------------------------------------------------------------
// Called on load. Addon should fully initalize or return error status
//-----------------------------------------------------------------------------
ADDON_STATUS CVisualizationNastyFFT::Create()
{
  plugin.setSize(Width(), Height());

  return ADDON_STATUS_NEED_SETTINGS;
}

ADDON_STATUS CVisualizationNastyFFT::GetStatus()
{
  return ADDON_STATUS_OK;
}

void CVisualizationNastyFFT::GetInfo(bool& wantsFreq, int& syncDelay)
{
  wantsFreq = true;
  syncDelay = 0;
}

bool CVisualizationNastyFFT::Start(int channels, int samplesPerSec, int bitsPerSample, std::string songName)
{
  return true;
}

//-- Render -------------------------------------------------------------------
// Called once per frame. Do all rendering here.
//-----------------------------------------------------------------------------
void CVisualizationNastyFFT::Render()
{
  plugin.render();
}

void CVisualizationNastyFFT::AudioData(const float* pAudioData, int iAudioDataLength, float *pFreqData, int iFreqDataLength)
{
  plugin.scopeEvent(pFreqData, iFreqDataLength);
}

//-- GetPresets ---------------------------------------------------------------
// Return a list of presets to Kodi for display
//-----------------------------------------------------------------------------
bool CVisualizationNastyFFT::GetPresets(std::vector<std::string>& presets)
{
  for (auto preset : g_presets)
    presets.push_back(preset);
  return true;
}

//-- LoadPreset -----------------------------------------------------------------
// Handle Kodi LoadPreset action
//-----------------------------------------------------------------------------
bool CVisualizationNastyFFT::LoadPreset(int select)
{
  plugin.loadPreset(select % g_presets.size());

  return true;
}

//-- GetActivePreset ----------------------------------------------------------------
// Return the index of the current playing preset
//-----------------------------------------------------------------------------
int CVisualizationNastyFFT::GetActivePreset()
{
  return plugin.presetIndex();
}

//-- SetSetting ---------------------------------------------------------------
// Set a specific Setting value (called from XBMC)
// !!! Add-on master function !!!
//-----------------------------------------------------------------------------
ADDON_STATUS CVisualizationNastyFFT::SetSetting(const std::string& settingName, const kodi::CSettingValue& settingValue)
{
  if (settingName.empty() || settingValue.empty())
    return ADDON_STATUS_UNKNOWN;

  ScenePreset *priv = plugin.scenePreset();

  if (settingName == "scale")
    priv->scale = (1 + settingValue.GetInt());
  else if (settingName == "invert")
    priv->cinvert = settingValue.GetBoolean();
  else if (settingName == "transient")
    priv->tranzient = settingValue.GetBoolean();
  else if (settingName == "cammove")
    priv->cammove = settingValue.GetBoolean();
  else if (settingName == "flatten")
    priv->flatten = settingValue.GetBoolean();
  else if (settingName == "eye_y")
    priv->cam_coords.y = settingValue.GetInt();
  else if (settingName == "eye_z")
    priv->cam_coords.z = settingValue.GetInt();
  else if (settingName == "brick_space_z")
    priv->step_z = (settingValue.GetInt() / 10.0f);
  else if (settingName == "brick_shiness")
    priv->shiness = (settingValue.GetInt() / 100.0f);
  else if (settingName == "floor_rotate_x")
    priv->rot_x = (0 + (settingValue.GetInt() * 36));
  else
    return ADDON_STATUS_UNKNOWN;
  return ADDON_STATUS_OK;
}


ADDONCREATOR(CVisualizationNastyFFT) // Don't touch this!
