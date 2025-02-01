
#include <stdio.h>
#include <stdlib.h>     /* atof */
#include <boost/algorithm/string.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <veins/modules/application/f2md/mdApplications/Izdihar.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace boost;

Izdihar::Izdihar(int version, double Threshold) :
        MDApplication(version) {
    this->Threshold = Threshold;
}

bool Izdihar::CheckNodeForReport(unsigned long myPseudonym,
        BasicSafetyMessage * bsm, BsmCheck * bsmCheck,
        NodeTable * detectedNodes) {

    bool checkFailed = false;
cout <<"SPEEDAPP";
    prntApp->incAll(mbTypes::intMbs[bsm->getSenderMbType()]);
    prntAppInst->incAll(mbTypes::intMbs[bsm->getSenderMbType()]);
    incrementDetailedFlags(bsm,bsmCheck,Threshold);

 double speed= mdmLib.calculateSpeedPtr(&bsm->getSenderSpeed());
               cout <<"speed is  "<<speed ;
                            if (speed<11.11){
                   unsigned long senderId = bsm->getSenderPseudonym();


                       double T_l=-1.01;

                       int indexTMO = getIndexTMO(senderId);
                   if(minFactor< 1){
                       double TMOadd = -0.0005*exp(10 * (1 - minFactor));
                       if (TMOadd >5.5066 & T_l<0){
                           T_l+=0.1;
                       }else{T_l+=TMOadd;
                   }
                       if (T_l < Threshold){

                                           checkFailed = true;
                           }
                       else {
                           checkFailed=false;}
                       }return checkFailed;


               }
                         else if(speed>=11.11){



        //////

        InterTest inter = bsmCheck->getIntersection();
        for (int var = 0; var < inter.getInterNum(); ++var) {
            if (inter.getInterValue(var) <= Threshold) {
                checkFailed = true;
                break;
            }
        }

        if (checkFailed || bsmCheck->getProximityPlausibility() <= Threshold
        || bsmCheck->getRangePlausibility() <= Threshold
                || bsmCheck->getPositionConsistancy() <= Threshold
                || bsmCheck->getPositionSpeedConsistancy() <= Threshold
                || bsmCheck->getPositionSpeedMaxConsistancy() <= Threshold
                || bsmCheck->getSpeedConsistancy() <= Threshold
                || bsmCheck->getSpeedPlausibility() <= Threshold
                || bsmCheck->getPositionPlausibility() <= Threshold
                || bsmCheck->getBeaconFrequency() <= Threshold
                || bsmCheck->getPositionHeadingConsistancy() <= Threshold
                || bsmCheck->getKalmanPSCP() <= Threshold
                || bsmCheck->getKalmanPSCS() <= Threshold
                || bsmCheck->getKalmanPSCSP() <= Threshold
                || bsmCheck->getKalmanPSCSS() <= Threshold
                || bsmCheck->getKalmanPCC() <= Threshold
                || bsmCheck->getKalmanPACS() <= Threshold
                || bsmCheck->getKalmanSCC() <= Threshold) {
            checkFailed = true;
        }



        }// izdihar add 28_1_2025


                            if (checkFailed) {
                                                           prntApp->incCumulFlags(mbTypes::intMbs[bsm->getSenderMbType()]);
                                                           prntAppInst->incCumulFlags(mbTypes::intMbs[bsm->getSenderMbType()]);
                                                           bsmCheck->setReported(true);



                                                       }
        return checkFailed;
        //incrementDetailedFlags(bsm,bsmCheck,Threshold);
        //incrementDetailedFlags(bsm,bsmCheck,Threshold);


    }

double Izdihar::getMinFactor() {
    return minFactor;
}


int Izdihar::addPseudoTMO(unsigned long pseudo) {
    if (TimeOutNum >= MAX_DETECTED_NODES_COOP) {
        removeOldestPseudoTMO();
    }
    TimeOut[TimeOutNum] = 0;
    UpdatedTMO[TimeOutNum] = simTime().dbl();
    PseudonymsToTMO[TimeOutNum] = pseudo;
    TimeOutNum++;

    //std::cout<<"TimeOutNum:"<<TimeOutNum<<"\n";

    return TimeOutNum - 1;
}

void Izdihar::removeOldestPseudoTMO() {
    int oldestIndex = 0;
    double oldestTime = UpdatedTMO[oldestIndex];
    for (int var = 0; var < TimeOutNum; ++var) {
        if (oldestTime > UpdatedTMO[var]) {
            oldestTime = UpdatedTMO[var];
            oldestIndex = var;
        }
    }
    removePseudoTMO(oldestIndex);
}

void Izdihar::removePseudoTMO(int index) {
    for (int var = index; var < TimeOutNum; ++var) {
        TimeOut[var] = TimeOut[var + 1];
    }
    TimeOutNum--;
}

int Izdihar::getIndexTMO(unsigned long pseudo) {
    for (int var = 0; var < TimeOutNum; ++var) {
        if (PseudonymsToTMO[var] == pseudo) {
            return var;
        }
    }
    return -1;
}





