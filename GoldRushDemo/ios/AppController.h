//
//  AppController.h
//  GoldRushDemo
//
//  Created by lee calf on 11-10-18.
//  Copyright TQND 2011. All rights reserved.
//

@class RootViewController;

@interface AppController : NSObject <UIAccelerometerDelegate, UIAlertViewDelegate, UITextFieldDelegate,UIApplicationDelegate> {
    UIWindow *window;
    RootViewController	*viewController;
}

@end

