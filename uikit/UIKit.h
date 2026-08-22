#ifndef UIKIT_H
#define UIKIT_H

/**
 * @file UIKit.h
 * @brief UIKit-Framework — A lightweight UIKit-style UI abstraction layer
 *        built on top of LVGL for embedded systems.
 *
 * Architecture:
 *   UIKit  = Application UI Abstraction
 *   LVGL   = Rendering / UI Engine
 *   Display/Input/Timer = Hardware / OS layer
 *
 * UIView is NOT a widget library — it is a thin wrapper around lv_obj_t.
 * Every UIKit component maps one-to-one onto an LVGL primitive.
 */

/* ── Core ─────────────────────────────────────────────────────────────────── */
#include "UIKitResult.h"
#include "UIEvent.h"
#include "UIView.h"
#include "UIViewRoot.h"

/* ── Components ───────────────────────────────────────────────────────────── */
#include "UILabel.h"
#include "UIButton.h"
#include "UISwitch.h"
#include "UISlider.h"
#include "UITextField.h"
#include "UIImageView.h"
#include "UIScrollView.h"
#include "UIProgressView.h"
#include "UIPageControl.h"

/* ── Controller ───────────────────────────────────────────────────────────── */
#include "UIViewController.h"
#include "UINavigationController.h"

/* ── Graphics ─────────────────────────────────────────────────────────────── */
#include "UITheme.h"
#include "UIViewAnimation.h"

/* ── Layout ───────────────────────────────────────────────────────────────── */
#include "UIStackView.h"

/* ── Platform ─────────────────────────────────────────────────────────────── */
#include "UIKitPlatform.h"

/* ── Framework lifecycle ──────────────────────────────────────────────────── */

/**
 * Initialize the UIKit framework.
 * Call once after lv_init() and display/input driver initialization.
 */
void UIKit_init(void);

/**
 * Process UIKit framework tasks.
 * Call in the main loop alongside lv_timer_handler().
 */
void UIKit_process(void);

#endif /* UIKIT_H */
