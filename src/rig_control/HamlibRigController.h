//=========================================================================
// Name:            HamlibRigController.h
// Purpose:         Controls radios using Hamlib library.
//
// Authors:         Mooneer Salem
// License:
//
//  All rights reserved.
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2.1,
//  as published by the Free Software Foundation.  This program is
//  distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
//  License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, see <http://www.gnu.org/licenses/>.
//
//=========================================================================

#ifndef HAMLIB_RIG_CONTROLLER_H
#define HAMLIB_RIG_CONTROLLER_H

#include <string>
#include <vector>
#include <mutex>

#include "util/ThreadedObject.h"
#include "IRigFrequencyController.h"
#include "IRigPttController.h"

extern "C" 
{
    #include <hamlib/rig.h>
}

class HamlibRigController : public ThreadedObject, public IRigFrequencyController, public IRigPttController
{
public:
    enum PttType 
    {
        PTT_VIA_CAT = 0,
        PTT_VIA_RTS,
        PTT_VIA_DTR,
        PTT_VIA_NONE,
    };
    
    HamlibRigController(std::string rigName, std::string serialPort, const int serialRate, const int civHex = 0, const PttType pttType = PTT_VIA_CAT, std::string pttSerialPort = std::string());
    virtual ~HamlibRigController();
    
    virtual void connect() override;
    virtual void disconnect() override;
    virtual void ptt(bool state) override;
    virtual void setFrequency(uint64_t frequency) override;
    virtual void setMode(IRigFrequencyController::Mode mode) override;
    virtual void requestCurrentFrequencyMode() override;
    
private:
    using RigList = std::vector<const struct rig_caps *>;
    
    std::string rigName_;
    std::string serialPort_;
    const int serialRate_;
    const int civHex_;
    const PttType pttType_;
    std::string pttSerialPort_;
    
    RIG* rig_;
    bool multipleVfos_;
    bool pttSet_;
    
    unsigned int rigNameToIndex_(std::string rigName);
    vfo_t getCurrentVfo_();
    void setFrequencyHelper_(vfo_t currVfo, uint64_t frequencyHz);
    void setModeHelper_(vfo_t currVfo, rmode_t mode);
    
    void connectImpl_();
    void disconnectImpl_();
    void pttImpl_(bool state);
    void setFrequencyImpl_(uint64_t frequencyHz);
    void setModeImpl_(IRigFrequencyController::Mode mode);
    void requestCurrentFrequencyModeImpl_();
    
    static RigList RigList_;
    static std::mutex RigListMutex_;
    
    static bool RigCompare_(const struct rig_caps *rig1, const struct rig_caps *rig2);
    static int BuildRigList_(const struct rig_caps *rig, rig_ptr_t);
};

#endif // HAMLIB_RIG_CONTROLLER_H