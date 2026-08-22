#include "UIViewRoot.h"
#include "UIView.h"
#include "UIKitPlatform.h"

UIView *UIViewRoot_create(void)
{
    UIView *root = UIView_create(NULL);
    if (!root) {
        return NULL;
    }

    /* Default frame: full screen (320x240 by convention) */
    UIView_set_frame(root, 0, 0, 320, 240);

    return root;
}
