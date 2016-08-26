//
//  Gyro.h
//  DragonTactic
//
//  Created by strarem599 ding on 8/26/16.
//
//

#import <Foundation/Foundation.h>
#import <CoreMotion/CoreMotion.h>

@interface Gyro : NSObject {
    int hp;
    CMMotionManager *motionManager;
    CMAttitude *referenceAttitude;
}

-(int)showMessage;
-(float)attitudeYaw;

@end
