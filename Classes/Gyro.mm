//
//  Gyro.m
//  DragonTactic
//
//  Created by strarem599 ding on 8/26/16.
//
//

#import "Gyro.h"

@implementation Gyro

-(id)init {
    if ((self =[super init])) {
        hp = 0;
        motionManager = [[CMMotionManager alloc] init];
        referenceAttitude = nil;
        
        CMDeviceMotion *deviceMotion = motionManager.deviceMotion;
        CMAttitude *attitude = deviceMotion.attitude;
        referenceAttitude = [attitude retain];
        [motionManager startDeviceMotionUpdates];
    }
    return self;
}

-(int)showMessage {
    hp++;
    return hp;
}

-(float)attitudeYaw {
    CMDeviceMotion *deviceMotion = motionManager.deviceMotion;
    CMAttitude *attitude = deviceMotion.attitude;
    return attitude.yaw;
}

@end
