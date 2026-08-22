#include "UIStackView.h"
#include "UIKitPlatform.h"

UIStackView *UIStackView_create(UIView *parent)
{
    UIStackView *stack = UIKit_calloc(1, sizeof(UIStackView));
    if (!stack) {
        return NULL;
    }

    lv_obj_t *parent_obj = parent ? parent->lv_obj : lv_scr_act();

    stack->base.lv_obj = lv_obj_create(parent_obj);
    if (!stack->base.lv_obj) {
        UIKit_free(stack);
        return NULL;
    }

    stack->base.superview = parent;
    stack->base.callbacks = NULL;
    stack->base.tag       = NULL;

    /* Default: horizontal flex with 0 spacing/padding */
    lv_obj_set_layout(stack->base.lv_obj, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(stack->base.lv_obj, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_all(stack->base.lv_obj, 0, 0);
    lv_obj_set_style_pad_row(stack->base.lv_obj, 0, 0);
    lv_obj_set_style_pad_column(stack->base.lv_obj, 0, 0);
    lv_obj_set_flex_align(stack->base.lv_obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);

    return stack;
}

void UIStackView_destroy(UIStackView *stack)
{
    if (!stack) {
        return;
    }
    UIView_destroy(&stack->base);
}

void UIStackView_set_axis(UIStackView *stack, UIStackAxis axis)
{
    if (!stack || !stack->base.lv_obj) {
        return;
    }
    lv_obj_set_flex_flow(stack->base.lv_obj,
                         axis == UIStackAxis_VERTICAL ? LV_FLEX_FLOW_COLUMN
                                                      : LV_FLEX_FLOW_ROW);
}

void UIStackView_set_spacing(UIStackView *stack, int32_t spacing)
{
    if (!stack || !stack->base.lv_obj) {
        return;
    }
    lv_obj_set_style_pad_row(stack->base.lv_obj, spacing, 0);
    lv_obj_set_style_pad_column(stack->base.lv_obj, spacing, 0);
}

void UIStackView_set_padding(UIStackView *stack, int32_t padding)
{
    if (!stack || !stack->base.lv_obj) {
        return;
    }
    lv_obj_set_style_pad_all(stack->base.lv_obj, padding, 0);
}

void UIStackView_add_arranged_subview(UIStackView *stack, UIView *child)
{
    if (!stack || !child) {
        return;
    }
    UIView_add_subview(&stack->base, child);
}

void UIStackView_set_alignment(UIStackView *stack, int32_t align)
{
    if (!stack || !stack->base.lv_obj) {
        return;
    }
    lv_flex_align_t main_place = LV_FLEX_ALIGN_START;
    lv_flex_align_t cross      = LV_FLEX_ALIGN_START;
    lv_flex_align_t track      = LV_FLEX_ALIGN_START;

    switch (align) {
    case 0:  cross = LV_FLEX_ALIGN_START;   break;
    case 1:  cross = LV_FLEX_ALIGN_CENTER;  break;
    case 2:  cross = LV_FLEX_ALIGN_START; break;
    case 3:  cross = LV_FLEX_ALIGN_END;     break;
    default: cross = LV_FLEX_ALIGN_START; break;
    }

    lv_obj_set_flex_align(stack->base.lv_obj, main_place, cross, track);
}
