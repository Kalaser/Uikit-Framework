# 真正要做的不是“重新实现 LVGL”

这是 UIKit 设计里最关键的一点。

不要：

<pre class="overflow-visible! px-0!" data-start="3466" data-end="3547"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>UIKit
 ↓
自己实现 renderer
 ↓
自己实现 event
 ↓
自己实现 animation
 ↓
自己实现 widget</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

而应该：

<pre class="overflow-visible! px-0!" data-start="3555" data-end="3611"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>UIKit
   ↓
LVGL
   ↓
Display / Input / Timer</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

也就是：

<pre class="overflow-visible! px-0!" data-start="3619" data-end="3693"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>UIKit
= Application UI Abstraction

LVGL
= Rendering/UI Engine</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

所以：

<pre class="overflow-visible! px-0!" data-start="3700" data-end="3715"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">UIView</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

本质上第一版只是：

<pre class="overflow-visible! px-0!" data-start="3728" data-end="3777"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼv">struct</span><span> </span><span class="ͼ11">UIView</span><span> {
    </span><span class="ͼ11">lv_obj_t</span><span> </span><span class="ͼ11">*lv_obj</span><span>;
};</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

这个思想非常重要。

---

# 九、Milestone 1：手搓 UIView

现在正式开始写 UIKit。

目录：

<pre class="overflow-visible! px-0!" data-start="3843" data-end="3905"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>uikit/
├── core/
│   ├── UIView.h
│   └── UIView.c</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

---

## UIView.h

<pre class="overflow-visible! px-0!" data-start="3925" data-end="4721"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼu">#ifndef UIKIT_VIEW_H</span><span>
</span><span class="ͼu">#define UIKIT_VIEW_H</span><span>

</span><span class="ͼu">#include "lvgl.h"</span><span>
</span><span class="ͼu">#include <stdint.h></span><span>
</span><span class="ͼu">#include <stdbool.h></span><span>

</span><span class="ͼv">typedef</span><span> </span><span class="ͼv">struct</span><span> </span><span class="ͼ11">UIView</span><span> </span><span class="ͼ11">UIView</span><span>;

</span><span class="ͼv">struct</span><span> </span><span class="ͼ11">UIView</span><span> {
    </span><span class="ͼ11">lv_obj_t</span><span> </span><span class="ͼ11">*lv_obj</span><span>;

    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">superview</span><span>;
};

</span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">UIView_create</span><span>(</span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">parent</span><span>);

</span><span class="ͼ11">void</span><span> </span><span class="ͼ11">UIView_destroy</span><span>(</span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">view</span><span>);

</span><span class="ͼ11">void</span><span> </span><span class="ͼ11">UIView_set_frame</span><span>(
    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">view</span><span>,
    </span><span class="ͼ11">int32_t</span><span> </span><span class="ͼ11">x</span><span>,
    </span><span class="ͼ11">int32_t</span><span> </span><span class="ͼ11">y</span><span>,
    </span><span class="ͼ11">int32_t</span><span> </span><span class="ͼ11">width</span><span>,
    </span><span class="ͼ11">int32_t</span><span> </span><span class="ͼ11">height</span><span>
);

</span><span class="ͼ11">void</span><span> </span><span class="ͼ11">UIView_get_frame</span><span>(
    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">view</span><span>,
    </span><span class="ͼ11">int32_t</span><span> </span><span class="ͼ11">*x</span><span>,
    </span><span class="ͼ11">int32_t</span><span> </span><span class="ͼ11">*y</span><span>,
    </span><span class="ͼ11">int32_t</span><span> </span><span class="ͼ11">*width</span><span>,
    </span><span class="ͼ11">int32_t</span><span> </span><span class="ͼ11">*height</span><span>
);

</span><span class="ͼ11">void</span><span> </span><span class="ͼ11">UIView_set_hidden</span><span>(
    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">view</span><span>,
    </span><span class="ͼ11">bool</span><span> </span><span class="ͼ11">hidden</span><span>
);

</span><span class="ͼ11">bool</span><span> </span><span class="ͼ11">UIView_is_hidden</span><span>(
    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">view</span><span>
);

</span><span class="ͼ11">void</span><span> </span><span class="ͼ11">UIView_add_subview</span><span>(
    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">parent</span><span>,
    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">child</span><span>
);

</span><span class="ͼ11">void</span><span> </span><span class="ͼ11">UIView_remove_from_superview</span><span>(
    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">view</span><span>
);

</span><span class="ͼ11">lv_obj_t</span><span> </span><span class="ͼ11">*UIView_native</span><span>(
    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">view</span><span>
);

</span><span class="ͼu">#endif</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

---

# 十、UIView.c

<pre class="overflow-visible! px-0!" data-start="4742" data-end="5233"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼu">#include "UIView.h"</span><span>
</span><span class="ͼu">#include <stdlib.h></span><span>

</span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">UIView_create</span><span>(</span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">parent</span><span>)
{
    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">view</span><span> </span><span class="ͼv">=</span><span> </span><span class="ͼ11">malloc</span><span>(</span><span class="ͼv">sizeof</span><span>(</span><span class="ͼ11">UIView</span><span>));

    </span><span class="ͼv">if</span><span> (</span><span class="ͼv">!</span><span class="ͼ11">view</span><span>) {
        </span><span class="ͼv">return</span><span> </span><span class="ͼy">NULL</span><span>;
    }

    </span><span class="ͼ11">lv_obj_t</span><span> </span><span class="ͼ11">*parent_obj</span><span> </span><span class="ͼv">=</span><span> </span><span class="ͼy">NULL</span><span>;

    </span><span class="ͼv">if</span><span> (</span><span class="ͼ11">parent</span><span>) {
        </span><span class="ͼ11">parent_obj</span><span> </span><span class="ͼv">=</span><span> </span><span class="ͼ11">parent</span><span class="ͼv">-></span><span class="ͼ11">lv_obj</span><span>;
    } </span><span class="ͼv">else</span><span> {
        </span><span class="ͼ11">parent_obj</span><span> </span><span class="ͼv">=</span><span> </span><span class="ͼ11">lv_scr_act</span><span>();
    }

    </span><span class="ͼ11">view</span><span class="ͼv">-></span><span class="ͼ11">lv_obj</span><span> </span><span class="ͼv">=</span><span> </span><span class="ͼ11">lv_obj_create</span><span>(</span><span class="ͼ11">parent_obj</span><span>);
    </span><span class="ͼ11">view</span><span class="ͼv">-></span><span class="ͼ11">superview</span><span> </span><span class="ͼv">=</span><span> </span><span class="ͼ11">parent</span><span>;

    </span><span class="ͼv">if</span><span> (</span><span class="ͼv">!</span><span class="ͼ11">view</span><span class="ͼv">-></span><span class="ͼ11">lv_obj</span><span>) {
        </span><span class="ͼ11">free</span><span>(</span><span class="ͼ11">view</span><span>);
        </span><span class="ͼv">return</span><span> </span><span class="ͼy">NULL</span><span>;
    }

    </span><span class="ͼv">return</span><span> </span><span class="ͼ11">view</span><span>;
}</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

然后：

<pre class="overflow-visible! px-0!" data-start="5240" data-end="5407"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">void</span><span> </span><span class="ͼ11">UIView_destroy</span><span>(</span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">view</span><span>)
{
    </span><span class="ͼv">if</span><span> (</span><span class="ͼv">!</span><span class="ͼ11">view</span><span>) {
        </span><span class="ͼv">return</span><span>;
    }

    </span><span class="ͼv">if</span><span> (</span><span class="ͼ11">view</span><span class="ͼv">-></span><span class="ͼ11">lv_obj</span><span>) {
        </span><span class="ͼ11">lv_obj_del</span><span>(</span><span class="ͼ11">view</span><span class="ͼv">-></span><span class="ͼ11">lv_obj</span><span>);
    }

    </span><span class="ͼ11">free</span><span>(</span><span class="ͼ11">view</span><span>);
}</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

---

## frame

<pre class="overflow-visible! px-0!" data-start="5424" data-end="5754"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">void</span><span> </span><span class="ͼ11">UIView_set_frame</span><span>(
    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">view</span><span>,
    </span><span class="ͼ11">int32_t</span><span> </span><span class="ͼ11">x</span><span>,
    </span><span class="ͼ11">int32_t</span><span> </span><span class="ͼ11">y</span><span>,
    </span><span class="ͼ11">int32_t</span><span> </span><span class="ͼ11">width</span><span>,
    </span><span class="ͼ11">int32_t</span><span> </span><span class="ͼ11">height</span><span>)
{
    </span><span class="ͼv">if</span><span> (</span><span class="ͼv">!</span><span class="ͼ11">view</span><span> </span><span class="ͼv">||</span><span> </span><span class="ͼv">!</span><span class="ͼ11">view</span><span class="ͼv">-></span><span class="ͼ11">lv_obj</span><span>) {
        </span><span class="ͼv">return</span><span>;
    }

    </span><span class="ͼ11">lv_obj_set_pos</span><span>(
        </span><span class="ͼ11">view</span><span class="ͼv">-></span><span class="ͼ11">lv_obj</span><span>,
        </span><span class="ͼ11">x</span><span>,
        </span><span class="ͼ11">y</span><span>
    );

    </span><span class="ͼ11">lv_obj_set_size</span><span>(
        </span><span class="ͼ11">view</span><span class="ͼv">-></span><span class="ͼ11">lv_obj</span><span>,
        </span><span class="ͼ11">width</span><span>,
        </span><span class="ͼ11">height</span><span>
    );
}</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

---

## hidden

<pre class="overflow-visible! px-0!" data-start="5772" data-end="6125"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">void</span><span> </span><span class="ͼ11">UIView_set_hidden</span><span>(
    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">view</span><span>,
    </span><span class="ͼ11">bool</span><span> </span><span class="ͼ11">hidden</span><span>)
{
    </span><span class="ͼv">if</span><span> (</span><span class="ͼv">!</span><span class="ͼ11">view</span><span> </span><span class="ͼv">||</span><span> </span><span class="ͼv">!</span><span class="ͼ11">view</span><span class="ͼv">-></span><span class="ͼ11">lv_obj</span><span>) {
        </span><span class="ͼv">return</span><span>;
    }

    </span><span class="ͼv">if</span><span> (</span><span class="ͼ11">hidden</span><span>) {
        </span><span class="ͼ11">lv_obj_add_flag</span><span>(
            </span><span class="ͼ11">view</span><span class="ͼv">-></span><span class="ͼ11">lv_obj</span><span>,
            </span><span class="ͼ11">LV_OBJ_FLAG_HIDDEN</span><span>
        );
    } </span><span class="ͼv">else</span><span> {
        </span><span class="ͼ11">lv_obj_clear_flag</span><span>(
            </span><span class="ͼ11">view</span><span class="ͼv">-></span><span class="ͼ11">lv_obj</span><span>,
            </span><span class="ͼ11">LV_OBJ_FLAG_HIDDEN</span><span>
        );
    }
}</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

---

# 十一、现在第一次使用 UIKit

<pre class="overflow-visible! px-0!" data-start="6152" data-end="6357"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼu">#include "lvgl.h"</span><span>
</span><span class="ͼu">#include "UIView.h"</span><span>

</span><span class="ͼ11">void</span><span> </span><span class="ͼ11">app_ui_init</span><span>(</span><span class="ͼ11">void</span><span>)
{
    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">root</span><span> </span><span class="ͼv">=</span><span> </span><span class="ͼ11">UIView_create</span><span>(</span><span class="ͼy">NULL</span><span>);

    </span><span class="ͼ11">UIView_set_frame</span><span>(
        </span><span class="ͼ11">root</span><span>,
        </span><span class="ͼy">0</span><span>,
        </span><span class="ͼy">0</span><span>,
        </span><span class="ͼy">320</span><span>,
        </span><span class="ͼy">240</span><span>
    );
}</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

此时实际上：

<pre class="overflow-visible! px-0!" data-start="6367" data-end="6404"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>UIView
  │
  └── lv_obj_t</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

---

# 十二、但是现在你会发现 UIView 很弱

比如：

<pre class="overflow-visible! px-0!" data-start="6441" data-end="6459"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>UIView</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

只能创建一个普通 `lv_obj`。

下一步就是：

<pre class="overflow-visible! px-0!" data-start="6489" data-end="6526"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>UILabel
UIButton
UISlider</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

---

# 十三、Milestone 2：UILabel

目录：

<pre class="overflow-visible! px-0!" data-start="6564" data-end="6680"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>uikit/
├── core/
│   ├── UIView.c
│   └── UIView.h
│
└── components/
    ├── UILabel.c
    └── UILabel.h</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

---

# 十四、UILabel.h

<pre class="overflow-visible! px-0!" data-start="6703" data-end="7032"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼu">#ifndef UIKIT_LABEL_H</span><span>
</span><span class="ͼu">#define UIKIT_LABEL_H</span><span>

</span><span class="ͼu">#include "../core/UIView.h"</span><span>

</span><span class="ͼv">typedef</span><span> </span><span class="ͼv">struct</span><span> </span><span class="ͼ11">UILabel</span><span> </span><span class="ͼ11">UILabel</span><span>;

</span><span class="ͼv">struct</span><span> </span><span class="ͼ11">UILabel</span><span> {
    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼ11">base</span><span>;
};

</span><span class="ͼ11">UILabel</span><span> </span><span class="ͼv">*</span><span class="ͼ11">UILabel_create</span><span>(
    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">parent</span><span>
);

</span><span class="ͼ11">void</span><span> </span><span class="ͼ11">UILabel_set_text</span><span>(
    </span><span class="ͼ11">UILabel</span><span> </span><span class="ͼv">*</span><span class="ͼ11">label</span><span>,
    </span><span class="ͼv">const</span><span> </span><span class="ͼ11">char</span><span> </span><span class="ͼ11">*text</span><span>
);

</span><span class="ͼv">const</span><span> </span><span class="ͼ11">char</span><span> </span><span class="ͼ11">*UILabel_get_text</span><span>(
    </span><span class="ͼ11">UILabel</span><span> </span><span class="ͼv">*</span><span class="ͼ11">label</span><span>
);

</span><span class="ͼu">#endif</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

---

# 十五、UILabel.c

<pre class="overflow-visible! px-0!" data-start="7055" data-end="7470"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼu">#include "UILabel.h"</span><span>

</span><span class="ͼu">#include <stdlib.h></span><span>

</span><span class="ͼ11">UILabel</span><span> </span><span class="ͼv">*</span><span class="ͼ11">UILabel_create</span><span>(
    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">parent</span><span>)
{
    </span><span class="ͼ11">UILabel</span><span> </span><span class="ͼv">*</span><span class="ͼ11">label</span><span> </span><span class="ͼv">=</span><span> </span><span class="ͼ11">malloc</span><span>(
        </span><span class="ͼv">sizeof</span><span>(</span><span class="ͼ11">UILabel</span><span>)
    );

    </span><span class="ͼv">if</span><span> (</span><span class="ͼv">!</span><span class="ͼ11">label</span><span>) {
        </span><span class="ͼv">return</span><span> </span><span class="ͼy">NULL</span><span>;
    }

    </span><span class="ͼ11">label</span><span class="ͼv">-></span><span class="ͼ11">base</span><span>.</span><span class="ͼ11">lv_obj</span><span> </span><span class="ͼv">=</span><span>
        </span><span class="ͼ11">lv_label_create</span><span>(
            </span><span class="ͼ11">parent</span><span>
                </span><span class="ͼv">?</span><span> </span><span class="ͼ11">parent</span><span class="ͼv">-></span><span class="ͼ11">lv_obj</span><span>
                : </span><span class="ͼ11">lv_scr_act</span><span>()
        );

    </span><span class="ͼ11">label</span><span class="ͼv">-></span><span class="ͼ11">base</span><span>.</span><span class="ͼ11">superview</span><span> </span><span class="ͼv">=</span><span>
        </span><span class="ͼ11">parent</span><span>;

    </span><span class="ͼv">return</span><span> </span><span class="ͼ11">label</span><span>;
}</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

设置文本：

<pre class="overflow-visible! px-0!" data-start="7479" data-end="7677"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">void</span><span> </span><span class="ͼ11">UILabel_set_text</span><span>(
    </span><span class="ͼ11">UILabel</span><span> </span><span class="ͼv">*</span><span class="ͼ11">label</span><span>,
    </span><span class="ͼv">const</span><span> </span><span class="ͼ11">char</span><span> </span><span class="ͼ11">*text</span><span>)
{
    </span><span class="ͼv">if</span><span> (</span><span class="ͼv">!</span><span class="ͼ11">label</span><span> </span><span class="ͼv">||</span><span> </span><span class="ͼv">!</span><span class="ͼ11">text</span><span>) {
        </span><span class="ͼv">return</span><span>;
    }

    </span><span class="ͼ11">lv_label_set_text</span><span>(
        </span><span class="ͼ11">label</span><span class="ͼv">-></span><span class="ͼ11">base</span><span>.</span><span class="ͼ11">lv_obj</span><span>,
        </span><span class="ͼ11">text</span><span>
    );
}</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

---

# 十六、这里开始出现一个非常重要的设计问题

现在你会发现：

<pre class="overflow-visible! px-0!" data-start="7717" data-end="7732"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">UIView</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

和：

<pre class="overflow-visible! px-0!" data-start="7738" data-end="7754"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">UILabel</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

有共同内容。

也就是：

<pre class="overflow-visible! px-0!" data-start="7770" data-end="7816"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>UILabel
 ├── lv_obj
 └── superview</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

本质上：

<pre class="overflow-visible! px-0!" data-start="7824" data-end="7855"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>UILabel IS-A UIView</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

所以你的 C API 需要开始考虑“继承”。

C 没有真正的类继承。

通常使用：

<pre class="overflow-visible! px-0!" data-start="7901" data-end="7946"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼv">struct</span><span> </span><span class="ͼ11">UILabel</span><span> {
    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼ11">base</span><span>;
};</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

于是：

<pre class="overflow-visible! px-0!" data-start="7953" data-end="8154"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>UILabel
┌───────────────────────┐
│ UIView base           │
│   ├── lv_obj          │
│   └── superview       │
│                       │
│ UILabel fields...     │
└───────────────────────┘</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

这是你的 UIKit C Core 非常重要的基础。

---

# 十七、Milestone 3：UIButton

<pre class="overflow-visible! px-0!" data-start="8216" data-end="8279"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼv">typedef</span><span> </span><span class="ͼv">struct</span><span> </span><span class="ͼ11">UIButton</span><span> {
    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼ11">base</span><span>;
} </span><span class="ͼ11">UIButton</span><span>;</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

创建：

<pre class="overflow-visible! px-0!" data-start="8286" data-end="8651"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">UIButton</span><span> </span><span class="ͼv">*</span><span class="ͼ11">UIButton_create</span><span>(
    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">parent</span><span>)
{
    </span><span class="ͼ11">UIButton</span><span> </span><span class="ͼv">*</span><span class="ͼ11">button</span><span> </span><span class="ͼv">=</span><span>
        </span><span class="ͼ11">malloc</span><span>(</span><span class="ͼv">sizeof</span><span>(</span><span class="ͼ11">UIButton</span><span>));

    </span><span class="ͼv">if</span><span> (</span><span class="ͼv">!</span><span class="ͼ11">button</span><span>) {
        </span><span class="ͼv">return</span><span> </span><span class="ͼy">NULL</span><span>;
    }

    </span><span class="ͼ11">button</span><span class="ͼv">-></span><span class="ͼ11">base</span><span>.</span><span class="ͼ11">lv_obj</span><span> </span><span class="ͼv">=</span><span>
        </span><span class="ͼ11">lv_btn_create</span><span>(
            </span><span class="ͼ11">parent</span><span>
                </span><span class="ͼv">?</span><span> </span><span class="ͼ11">parent</span><span class="ͼv">-></span><span class="ͼ11">lv_obj</span><span>
                : </span><span class="ͼ11">lv_scr_act</span><span>()
        );

    </span><span class="ͼ11">button</span><span class="ͼv">-></span><span class="ͼ11">base</span><span>.</span><span class="ͼ11">superview</span><span> </span><span class="ͼv">=</span><span> </span><span class="ͼ11">parent</span><span>;

    </span><span class="ͼv">return</span><span> </span><span class="ͼ11">button</span><span>;
}</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

---

# 十八、Button 文本

LVGL 8 的 Button 本身不是 Label，所以：

<pre class="overflow-visible! px-0!" data-start="8706" data-end="8780"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>UIButton
   │
   └── lv_btn
          │
          └── lv_label</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

所以：

<pre class="overflow-visible! px-0!" data-start="8787" data-end="8855"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼv">struct</span><span> </span><span class="ͼ11">UIButton</span><span> {
    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼ11">base</span><span>;

    </span><span class="ͼ11">lv_obj_t</span><span> </span><span class="ͼ11">*label</span><span>;
};</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

创建：

<pre class="overflow-visible! px-0!" data-start="8862" data-end="9436"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">UIButton</span><span> </span><span class="ͼv">*</span><span class="ͼ11">UIButton_create</span><span>(
    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">parent</span><span>)
{
    </span><span class="ͼ11">UIButton</span><span> </span><span class="ͼv">*</span><span class="ͼ11">button</span><span> </span><span class="ͼv">=</span><span>
        </span><span class="ͼ11">malloc</span><span>(</span><span class="ͼv">sizeof</span><span>(</span><span class="ͼ11">UIButton</span><span>));

    </span><span class="ͼv">if</span><span> (</span><span class="ͼv">!</span><span class="ͼ11">button</span><span>) {
        </span><span class="ͼv">return</span><span> </span><span class="ͼy">NULL</span><span>;
    }

    </span><span class="ͼ11">button</span><span class="ͼv">-></span><span class="ͼ11">base</span><span>.</span><span class="ͼ11">lv_obj</span><span> </span><span class="ͼv">=</span><span>
        </span><span class="ͼ11">lv_btn_create</span><span>(
            </span><span class="ͼ11">parent</span><span>
                </span><span class="ͼv">?</span><span> </span><span class="ͼ11">parent</span><span class="ͼv">-></span><span class="ͼ11">lv_obj</span><span>
                : </span><span class="ͼ11">lv_scr_act</span><span>()
        );

    </span><span class="ͼ11">button</span><span class="ͼv">-></span><span class="ͼ11">base</span><span>.</span><span class="ͼ11">superview</span><span> </span><span class="ͼv">=</span><span> </span><span class="ͼ11">parent</span><span>;

    </span><span class="ͼ11">button</span><span class="ͼv">-></span><span class="ͼ11">label</span><span> </span><span class="ͼv">=</span><span>
        </span><span class="ͼ11">lv_label_create</span><span>(
            </span><span class="ͼ11">button</span><span class="ͼv">-></span><span class="ͼ11">base</span><span>.</span><span class="ͼ11">lv_obj</span><span>
        );

    </span><span class="ͼ11">lv_label_set_text</span><span>(
        </span><span class="ͼ11">button</span><span class="ͼv">-></span><span class="ͼ11">label</span><span>,
        </span><span class="ͼz">"Button"</span><span>
    );

    </span><span class="ͼ11">lv_obj_center</span><span>(
        </span><span class="ͼ11">button</span><span class="ͼv">-></span><span class="ͼ11">label</span><span>
    );

    </span><span class="ͼv">return</span><span> </span><span class="ͼ11">button</span><span>;
}</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

---

# 十九、Milestone 4：事件系统

这一步很重要。

不能让应用直接写：

<pre class="overflow-visible! px-0!" data-start="9486" data-end="9519"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">lv_obj_add_event_cb</span><span>(...)</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

否则你的 UIKit 没意义了。

应该：

<pre class="overflow-visible! px-0!" data-start="9544" data-end="9616"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">UIButton_set_action</span><span>(
    </span><span class="ͼ11">button</span><span>,
    </span><span class="ͼ11">callback</span><span>,
    </span><span class="ͼ11">user_data</span><span>
);</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

---

# 二十、定义 UIKit Event

<pre class="overflow-visible! px-0!" data-start="9644" data-end="9779"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼv">typedef</span><span> </span><span class="ͼv">enum</span><span> {
    </span><span class="ͼ11">UI_EVENT_CLICK</span><span> </span><span class="ͼv">=</span><span> </span><span class="ͼy">0</span><span>,
    </span><span class="ͼ11">UI_EVENT_PRESSED</span><span>,
    </span><span class="ͼ11">UI_EVENT_RELEASED</span><span>,
    </span><span class="ͼ11">UI_EVENT_VALUE_CHANGED</span><span>,
} </span><span class="ͼ11">UIEventType</span><span>;</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

回调：

<pre class="overflow-visible! px-0!" data-start="9786" data-end="9893"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼv">typedef</span><span> </span><span class="ͼ11">void</span><span> (</span><span class="ͼv">*</span><span class="ͼ11">UIEventCallback</span><span>)(
    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">sender</span><span>,
    </span><span class="ͼ11">UIEventType</span><span> </span><span class="ͼ11">event</span><span>,
    </span><span class="ͼ11">void</span><span> </span><span class="ͼ11">*user_data</span><span>
);</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

---

# 二十一、UIButton 增加 callback

<pre class="overflow-visible! px-0!" data-start="9928" data-end="10068"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼv">typedef</span><span> </span><span class="ͼv">struct</span><span> </span><span class="ͼ11">UIButton</span><span> {

    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼ11">base</span><span>;

    </span><span class="ͼ11">lv_obj_t</span><span> </span><span class="ͼ11">*label</span><span>;

    </span><span class="ͼ11">UIEventCallback</span><span> </span><span class="ͼ11">callback</span><span>;

    </span><span class="ͼ11">void</span><span> </span><span class="ͼ11">*user_data</span><span>;

} </span><span class="ͼ11">UIButton</span><span>;</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

---

# 二十二、LVGL → UIKit Event Adapter

这是 UIKit 的核心。

<pre class="overflow-visible! px-0!" data-start="10124" data-end="10856"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼv">static</span><span> </span><span class="ͼ11">void</span><span> </span><span class="ͼ11">UIButton_lv_event</span><span>(
    </span><span class="ͼ11">lv_event_t</span><span> </span><span class="ͼ11">*event</span><span>)
{
    </span><span class="ͼ11">UIButton</span><span> </span><span class="ͼv">*</span><span class="ͼ11">button</span><span> </span><span class="ͼv">=</span><span>
        </span><span class="ͼ11">lv_event_get_user_data</span><span>(</span><span class="ͼ11">event</span><span>);

    </span><span class="ͼv">if</span><span> (</span><span class="ͼv">!</span><span class="ͼ11">button</span><span>) {
        </span><span class="ͼv">return</span><span>;
    }

    </span><span class="ͼ11">lv_event_code_t</span><span> </span><span class="ͼ11">code</span><span> </span><span class="ͼv">=</span><span>
        </span><span class="ͼ11">lv_event_get_code</span><span>(</span><span class="ͼ11">event</span><span>);

    </span><span class="ͼ11">UIEventType</span><span> </span><span class="ͼ11">type</span><span>;

    </span><span class="ͼv">switch</span><span> (</span><span class="ͼ11">code</span><span>) {

        </span><span class="ͼv">case</span><span> </span><span class="ͼ11">LV_EVENT_CLICKED</span><span>:
            </span><span class="ͼ11">type</span><span> </span><span class="ͼv">=</span><span> </span><span class="ͼ11">UI_EVENT_CLICK</span><span>;
            </span><span class="ͼv">break</span><span>;

        </span><span class="ͼv">case</span><span> </span><span class="ͼ11">LV_EVENT_PRESSED</span><span>:
            </span><span class="ͼ11">type</span><span> </span><span class="ͼv">=</span><span> </span><span class="ͼ11">UI_EVENT_PRESSED</span><span>;
            </span><span class="ͼv">break</span><span>;

        </span><span class="ͼv">case</span><span> </span><span class="ͼ11">LV_EVENT_RELEASED</span><span>:
            </span><span class="ͼ11">type</span><span> </span><span class="ͼv">=</span><span> </span><span class="ͼ11">UI_EVENT_RELEASED</span><span>;
            </span><span class="ͼv">break</span><span>;

        </span><span class="ͼv">default</span><span>:
            </span><span class="ͼv">return</span><span>;
    }

    </span><span class="ͼv">if</span><span> (</span><span class="ͼ11">button</span><span class="ͼv">-></span><span class="ͼ11">callback</span><span>) {

        </span><span class="ͼ11">button</span><span class="ͼv">-></span><span class="ͼ11">callback</span><span>(
            </span><span class="ͼv">&</span><span class="ͼ11">button</span><span class="ͼv">-></span><span class="ͼ11">base</span><span>,
            </span><span class="ͼ11">type</span><span>,
            </span><span class="ͼ11">button</span><span class="ͼv">-></span><span class="ͼ11">user_data</span><span>
        );
    }
}</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

绑定：

<pre class="overflow-visible! px-0!" data-start="10863" data-end="10972"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">lv_obj_add_event_cb</span><span>(
    </span><span class="ͼ11">button</span><span class="ͼv">-></span><span class="ͼ11">base</span><span>.</span><span class="ͼ11">lv_obj</span><span>,
    </span><span class="ͼ11">UIButton_lv_event</span><span>,
    </span><span class="ͼ11">LV_EVENT_ALL</span><span>,
    </span><span class="ͼ11">button</span><span>
);</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

现在结构变成：

<pre class="overflow-visible! px-0!" data-start="10983" data-end="11099"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>LVGL
 │
 │ LV_EVENT_CLICKED
 ▼
UIButton Adapter
 │
 │ UI_EVENT_CLICK
 ▼
UIKit Callback
 │
 ▼
Application</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

这一步完成后，你才真正开始拥有一个 Framework。

---

# 二十三、应用代码

<pre class="overflow-visible! px-0!" data-start="11148" data-end="11330"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼv">static</span><span> </span><span class="ͼ11">void</span><span> </span><span class="ͼ11">button_callback</span><span>(
    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">sender</span><span>,
    </span><span class="ͼ11">UIEventType</span><span> </span><span class="ͼ11">event</span><span>,
    </span><span class="ͼ11">void</span><span> </span><span class="ͼ11">*user_data</span><span>)
{
    </span><span class="ͼv">if</span><span> (</span><span class="ͼ11">event</span><span> </span><span class="ͼv">==</span><span> </span><span class="ͼ11">UI_EVENT_CLICK</span><span>) {
        </span><span class="ͼ11">printf</span><span>(</span><span class="ͼz">"Button clicked\n"</span><span>);
    }
}</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

使用：

<pre class="overflow-visible! px-0!" data-start="11337" data-end="11458"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">UIButton</span><span> </span><span class="ͼv">*</span><span class="ͼ11">button</span><span> </span><span class="ͼv">=</span><span>
    </span><span class="ͼ11">UIButton_create</span><span>(</span><span class="ͼy">NULL</span><span>);

</span><span class="ͼ11">UIButton_set_action</span><span>(
    </span><span class="ͼ11">button</span><span>,
    </span><span class="ͼ11">button_callback</span><span>,
    </span><span class="ͼy">NULL</span><span>
);</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

应用已经完全不需要知道：

<pre class="overflow-visible! px-0!" data-start="11474" data-end="11510"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">lv_event_t</span><span>
</span><span class="ͼ11">LV_EVENT_CLICKED</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

---

# 二十四、Milestone 5：View Hierarchy

现在加入：

<pre class="overflow-visible! px-0!" data-start="11558" data-end="11587"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">UIView_add_subview</span><span>()</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

<pre class="overflow-visible! px-0!" data-start="11589" data-end="11825"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">void</span><span> </span><span class="ͼ11">UIView_add_subview</span><span>(
    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">parent</span><span>,
    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">child</span><span>)
{
    </span><span class="ͼv">if</span><span> (</span><span class="ͼv">!</span><span class="ͼ11">parent</span><span> </span><span class="ͼv">||</span><span> </span><span class="ͼv">!</span><span class="ͼ11">child</span><span>) {
        </span><span class="ͼv">return</span><span>;
    }

    </span><span class="ͼ11">child</span><span class="ͼv">-></span><span class="ͼ11">superview</span><span> </span><span class="ͼv">=</span><span> </span><span class="ͼ11">parent</span><span>;

    </span><span class="ͼ11">lv_obj_set_parent</span><span>(
        </span><span class="ͼ11">child</span><span class="ͼv">-></span><span class="ͼ11">lv_obj</span><span>,
        </span><span class="ͼ11">parent</span><span class="ͼv">-></span><span class="ͼ11">lv_obj</span><span>
    );
}</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

之后：

<pre class="overflow-visible! px-0!" data-start="11832" data-end="11971"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">root</span><span> </span><span class="ͼv">=</span><span>
    </span><span class="ͼ11">UIView_create</span><span>(</span><span class="ͼy">NULL</span><span>);

</span><span class="ͼ11">UILabel</span><span> </span><span class="ͼv">*</span><span class="ͼ11">title</span><span> </span><span class="ͼv">=</span><span>
    </span><span class="ͼ11">UILabel_create</span><span>(</span><span class="ͼ11">root</span><span>);

</span><span class="ͼ11">UIButton</span><span> </span><span class="ͼv">*</span><span class="ͼ11">button</span><span> </span><span class="ͼv">=</span><span>
    </span><span class="ͼ11">UIButton_create</span><span>(</span><span class="ͼ11">root</span><span>);</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

得到：

<pre class="overflow-visible! px-0!" data-start="11978" data-end="12030"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>root UIView
│
├── UILabel
│
└── UIButton</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

这时候已经有真正的：

<pre class="overflow-visible! px-0!" data-start="12044" data-end="12072"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>UIView Hierarchy</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

---

# 二十五、下一步不是立即写 Navigation

应该先解决一个核心问题：

# 生命周期

否则未来：

<pre class="overflow-visible! px-0!" data-start="12135" data-end="12196"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>UIViewController
 ↓
UIView
 ↓
UIButton
 ↓
UILabel</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

销毁时会非常混乱。

---

# 二十六、Milestone 6：UIViewController

定义：

<pre class="overflow-visible! px-0!" data-start="12255" data-end="12355"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼv">typedef</span><span> </span><span class="ͼv">struct</span><span> </span><span class="ͼ11">UIViewController</span><span> {

    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">view</span><span>;

    </span><span class="ͼ11">bool</span><span> </span><span class="ͼ11">loaded</span><span>;

} </span><span class="ͼ11">UIViewController</span><span>;</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

初始化：

<pre class="overflow-visible! px-0!" data-start="12363" data-end="12585"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">UIViewController</span><span> </span><span class="ͼv">*</span><span>
</span><span class="ͼ11">UIViewController_create</span><span>(</span><span class="ͼ11">void</span><span>)
{
    </span><span class="ͼ11">UIViewController</span><span> </span><span class="ͼv">*</span><span class="ͼ11">vc</span><span> </span><span class="ͼv">=</span><span>
        </span><span class="ͼ11">malloc</span><span>(</span><span class="ͼv">sizeof</span><span>(</span><span class="ͼv">*</span><span class="ͼ11">vc</span><span>));

    </span><span class="ͼv">if</span><span> (</span><span class="ͼv">!</span><span class="ͼ11">vc</span><span>) {
        </span><span class="ͼv">return</span><span> </span><span class="ͼy">NULL</span><span>;
    }

    </span><span class="ͼ11">vc</span><span class="ͼv">-></span><span class="ͼ11">view</span><span> </span><span class="ͼv">=</span><span> </span><span class="ͼy">NULL</span><span>;
    </span><span class="ͼ11">vc</span><span class="ͼv">-></span><span class="ͼ11">loaded</span><span> </span><span class="ͼv">=</span><span> </span><span class="ͼy">false</span><span>;

    </span><span class="ͼv">return</span><span> </span><span class="ͼ11">vc</span><span>;
}</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

---

# 二十七、生命周期函数

<pre class="overflow-visible! px-0!" data-start="12606" data-end="12825"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">void</span><span> </span><span class="ͼ11">UIViewController_loadView</span><span>(
    </span><span class="ͼ11">UIViewController</span><span> </span><span class="ͼv">*</span><span class="ͼ11">vc</span><span>)
{
    </span><span class="ͼv">if</span><span> (</span><span class="ͼv">!</span><span class="ͼ11">vc</span><span>) {
        </span><span class="ͼv">return</span><span>;
    }

    </span><span class="ͼv">if</span><span> (</span><span class="ͼ11">vc</span><span class="ͼv">-></span><span class="ͼ11">view</span><span>) {
        </span><span class="ͼv">return</span><span>;
    }

    </span><span class="ͼ11">vc</span><span class="ͼv">-></span><span class="ͼ11">view</span><span> </span><span class="ͼv">=</span><span>
        </span><span class="ͼ11">UIView_create</span><span>(</span><span class="ͼy">NULL</span><span>);

    </span><span class="ͼ11">vc</span><span class="ͼv">-></span><span class="ͼ11">loaded</span><span> </span><span class="ͼv">=</span><span> </span><span class="ͼy">true</span><span>;
}</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

然后：

<pre class="overflow-visible! px-0!" data-start="12832" data-end="12930"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">void</span><span> </span><span class="ͼ11">UIViewController_viewDidLoad</span><span>(
    </span><span class="ͼ11">UIViewController</span><span> </span><span class="ͼv">*</span><span class="ͼ11">vc</span><span>)
{
    </span><span class="ͼt">/* override point */</span><span>
}</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

---

# 二十八、真正需要的是函数指针

因为 C 没有 virtual function。

定义：

<pre class="overflow-visible! px-0!" data-start="12987" data-end="13114"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼv">typedef</span><span> </span><span class="ͼv">struct</span><span> </span><span class="ͼ11">UIViewController</span><span> </span><span class="ͼ11">UIViewController</span><span>;

</span><span class="ͼv">typedef</span><span> </span><span class="ͼ11">void</span><span> (</span><span class="ͼv">*</span><span class="ͼ11">UIViewControllerMethod</span><span>)(
    </span><span class="ͼ11">UIViewController</span><span> </span><span class="ͼv">*</span><span class="ͼ11">vc</span><span>
);</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

然后：

<pre class="overflow-visible! px-0!" data-start="13121" data-end="13456"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼv">struct</span><span> </span><span class="ͼ11">UIViewController</span><span> {

    </span><span class="ͼ11">UIView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">view</span><span>;

    </span><span class="ͼ11">bool</span><span> </span><span class="ͼ11">loaded</span><span>;

    </span><span class="ͼ11">UIViewControllerMethod</span><span>
        </span><span class="ͼ11">viewDidLoad</span><span>;

    </span><span class="ͼ11">UIViewControllerMethod</span><span>
        </span><span class="ͼ11">viewWillAppear</span><span>;

    </span><span class="ͼ11">UIViewControllerMethod</span><span>
        </span><span class="ͼ11">viewDidAppear</span><span>;

    </span><span class="ͼ11">UIViewControllerMethod</span><span>
        </span><span class="ͼ11">viewWillDisappear</span><span>;

    </span><span class="ͼ11">UIViewControllerMethod</span><span>
        </span><span class="ͼ11">viewDidDisappear</span><span>;
};</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

这就是你 C 版本的“虚函数”。

---

# 二十九、现在你终于可以做 Navigation

数据结构：

<pre class="overflow-visible! px-0!" data-start="13515" data-end="13649"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼv">typedef</span><span> </span><span class="ͼv">struct</span><span> {

    </span><span class="ͼ11">UIViewController</span><span> </span><span class="ͼv">**</span><span class="ͼ11">controllers</span><span>;

    </span><span class="ͼ11">uint16_t</span><span> </span><span class="ͼ11">count</span><span>;

    </span><span class="ͼ11">uint16_t</span><span> </span><span class="ͼ11">capacity</span><span>;

} </span><span class="ͼ11">UINavigationController</span><span>;</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

实际就是：

<pre class="overflow-visible! px-0!" data-start="13658" data-end="13855"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>NavigationController
        │
        ▼
┌──────────────────────────┐
│ controller[0] Home       │
│ controller[1] Settings   │
│ controller[2] About      │
└──────────────────────────┘</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

---

# 三十、Push

<pre class="overflow-visible! px-0!" data-start="13873" data-end="14499"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">void</span><span> </span><span class="ͼ11">UINavigationController_push</span><span>(
    </span><span class="ͼ11">UINavigationController</span><span> </span><span class="ͼv">*</span><span class="ͼ11">nav</span><span>,
    </span><span class="ͼ11">UIViewController</span><span> </span><span class="ͼv">*</span><span class="ͼ11">vc</span><span>)
{
    </span><span class="ͼv">if</span><span> (</span><span class="ͼv">!</span><span class="ͼ11">nav</span><span> </span><span class="ͼv">||</span><span> </span><span class="ͼv">!</span><span class="ͼ11">vc</span><span>) {
        </span><span class="ͼv">return</span><span>;
    }

    </span><span class="ͼv">if</span><span> (</span><span class="ͼ11">nav</span><span class="ͼv">-></span><span class="ͼ11">count</span><span> </span><span class="ͼv">>=</span><span> </span><span class="ͼ11">nav</span><span class="ͼv">-></span><span class="ͼ11">capacity</span><span>) {
        </span><span class="ͼv">return</span><span>;
    }

    </span><span class="ͼ11">UIViewController</span><span> </span><span class="ͼv">*</span><span class="ͼ11">old</span><span> </span><span class="ͼv">=</span><span>
        </span><span class="ͼ11">nav</span><span class="ͼv">-></span><span class="ͼ11">controllers</span><span>[
            </span><span class="ͼ11">nav</span><span class="ͼv">-></span><span class="ͼ11">count</span><span> </span><span class="ͼv">-</span><span> </span><span class="ͼy">1</span><span>
        ];

    </span><span class="ͼv">if</span><span> (</span><span class="ͼ11">old</span><span> </span><span class="ͼv">&&</span><span>
        </span><span class="ͼ11">old</span><span class="ͼv">-></span><span class="ͼ11">viewWillDisappear</span><span>) {
        </span><span class="ͼ11">old</span><span class="ͼv">-></span><span class="ͼ11">viewWillDisappear</span><span>(</span><span class="ͼ11">old</span><span>);
    }

    </span><span class="ͼ11">nav</span><span class="ͼv">-></span><span class="ͼ11">controllers</span><span>[
        </span><span class="ͼ11">nav</span><span class="ͼv">-></span><span class="ͼ11">count</span><span class="ͼv">++</span><span>
    ] </span><span class="ͼv">=</span><span> </span><span class="ͼ11">vc</span><span>;

    </span><span class="ͼ11">UIViewController_loadView</span><span>(</span><span class="ͼ11">vc</span><span>);

    </span><span class="ͼv">if</span><span> (</span><span class="ͼ11">vc</span><span class="ͼv">-></span><span class="ͼ11">viewWillAppear</span><span>) {
        </span><span class="ͼ11">vc</span><span class="ͼv">-></span><span class="ͼ11">viewWillAppear</span><span>(</span><span class="ͼ11">vc</span><span>);
    }

    </span><span class="ͼv">if</span><span> (</span><span class="ͼ11">vc</span><span class="ͼv">-></span><span class="ͼ11">viewDidAppear</span><span>) {
        </span><span class="ͼ11">vc</span><span class="ͼv">-></span><span class="ͼ11">viewDidAppear</span><span>(</span><span class="ͼ11">vc</span><span>);
    }
}</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

---

# 三十一、这里你要开始建立一个完整的 UI Runtime

你的整体架构开始变成：

<pre class="overflow-visible! px-0!" data-start="14551" data-end="15348"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>                    Application
                         │
                         ▼
                UIViewController
                         │
                         ▼
              UINavigationController
                         │
                         ▼
                    UIView Tree
                         │
          ┌──────────────┼──────────────┐
          ▼              ▼              ▼
       UILabel        UIButton       UISlider
          │              │              │
          └──────────────┼──────────────┘
                         ▼
                    UIKit Core
                         │
              ┌──────────┼─────────┐
              ▼          ▼         ▼
           Event      Lifecycle  Layout
              │
              ▼
             LVGL</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

这就是你最终想要的东西。

---

# 三十二、不要过早写 C++

这是我对你这个项目最重要的一条建议：

不要第一天就：

<pre class="overflow-visible! px-0!" data-start="15415" data-end="15467"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼv">class</span><span> </span><span class="ͼ11">UIView</span><span>
</span><span class="ͼv">class</span><span> </span><span class="ͼ11">UIButton</span><span>
</span><span class="ͼv">class</span><span> UILabel</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

先把 C Core 做出来。

原因是：

<pre class="overflow-visible! px-0!" data-start="15491" data-end="15599"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>C Core
 │
 ├── ABI 稳定
 ├── MCU 友好
 ├── NuttX 友好
 ├── FreeRTOS 友好
 ├── 无 STL
 ├── 无异常
 └── 无 RTTI</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

之后再：

<pre class="overflow-visible! px-0!" data-start="15607" data-end="15640"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>C API
  ↓
C++ Wrapper</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

---

# 三十三、C++ Wrapper 怎么长出来

例如 C API：

<pre class="overflow-visible! px-0!" data-start="15683" data-end="15858"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">UIButton</span><span> </span><span class="ͼv">*</span><span class="ͼ11">UIButton_create</span><span>();
</span><span class="ͼ11">void</span><span> </span><span class="ͼ11">UIButton_set_title</span><span>(
    </span><span class="ͼ11">UIButton</span><span> </span><span class="ͼv">*</span><span>,
    </span><span class="ͼv">const</span><span> </span><span class="ͼ11">char</span><span> </span><span class="ͼ11">*</span><span>
);
</span><span class="ͼ11">void</span><span> </span><span class="ͼ11">UIButton_set_action</span><span>(
    </span><span class="ͼ11">UIButton</span><span> </span><span class="ͼv">*</span><span>,
    </span><span class="ͼ11">UIEventCallback</span><span>,
    </span><span class="ͼ11">void</span><span> </span><span class="ͼ11">*</span><span>
);</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

C++：

<pre class="overflow-visible! px-0!" data-start="15866" data-end="16238"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼv">class</span><span> </span><span class="ͼ11">UIButton</span><span> {
</span><span class="ͼv">public</span><span>:

    </span><span class="ͼ11">UIButton</span><span>()
    {
        </span><span class="ͼ11">handle_</span><span> =
            </span><span class="ͼ11">UIButton_create</span><span>();
    }

    </span><span class="ͼ11">~UIButton</span><span>()
    {
        </span><span class="ͼ11">UIButton_destroy</span><span>(
            </span><span class="ͼ11">handle_</span><span>
        );
    }

    </span><span class="ͼ11">void</span><span> setTitle(
        </span><span class="ͼv">const</span><span> </span><span class="ͼ11">char</span><span> *</span><span class="ͼ11">text</span><span>)
    {
        </span><span class="ͼ11">UIButton_set_title</span><span>(
            </span><span class="ͼ11">handle_</span><span>,
            </span><span class="ͼ11">text</span><span>
        );
    }

</span><span class="ͼv">private</span><span>:

    ::</span><span class="ͼ11">UIButton</span><span> *handle_;
};</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

然后：

<pre class="overflow-visible! px-0!" data-start="16245" data-end="16299"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">UIButton</span><span> </span><span class="ͼ11">button</span><span>;

</span><span class="ͼ11">button</span><span>.setTitle(</span><span class="ͼz">"Start"</span><span>);</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

---

# 三十四、再往后才是 RAII

之后：

<pre class="overflow-visible! px-0!" data-start="16329" data-end="16411"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼv">auto</span><span> </span><span class="ͼ11">button</span><span> =
    UIButton::</span><span class="ͼ11">create</span><span>();

</span><span class="ͼ11">button</span><span>->setTitle(
    </span><span class="ͼz">"Start"</span><span>
);</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

甚至：

<pre class="overflow-visible! px-0!" data-start="16418" data-end="16510"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">button</span><span>->onClick(
    [](</span><span class="ͼ11">UIButton</span><span> &</span><span class="ͼ11">button</span><span>) {
        </span><span class="ͼ11">printf</span><span>(</span><span class="ͼz">"clicked</span><span class="ͼ15">\n</span><span class="ͼz">"</span><span>);
    }
);</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

这时候 C++ API 就非常漂亮了。

---

# 三十五、然后再做 Theme

Theme 不应该第一天做。

应该先有：

<pre class="overflow-visible! px-0!" data-start="16579" data-end="16614"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>UIView
UIButton
UILabel</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

以后统一：

<pre class="overflow-visible! px-0!" data-start="16623" data-end="16901"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼv">typedef</span><span> </span><span class="ͼv">struct</span><span> </span><span class="ͼ11">UITheme</span><span> {

    </span><span class="ͼ11">lv_color_t</span><span> </span><span class="ͼ11">background</span><span>;

    </span><span class="ͼ11">lv_color_t</span><span> </span><span class="ͼ11">foreground</span><span>;

    </span><span class="ͼ11">lv_color_t</span><span> </span><span class="ͼ11">primary</span><span>;

    </span><span class="ͼ11">lv_color_t</span><span> </span><span class="ͼ11">secondary</span><span>;

    </span><span class="ͼv">const</span><span> </span><span class="ͼ11">lv_font_t</span><span> </span><span class="ͼ11">*font_small</span><span>;

    </span><span class="ͼv">const</span><span> </span><span class="ͼ11">lv_font_t</span><span> </span><span class="ͼ11">*font_normal</span><span>;

    </span><span class="ͼv">const</span><span> </span><span class="ͼ11">lv_font_t</span><span> </span><span class="ͼ11">*font_large</span><span>;

    </span><span class="ͼ11">uint16_t</span><span> </span><span class="ͼ11">radius</span><span>;

} </span><span class="ͼ11">UITheme</span><span>;</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

然后：

<pre class="overflow-visible! px-0!" data-start="16908" data-end="16947"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">UIKit_set_theme</span><span>(
    </span><span class="ͼv">&</span><span class="ͼ11">theme</span><span>
);</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

UIButton 自动获取：

<pre class="overflow-visible! px-0!" data-start="16965" data-end="17042"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>Theme
  │
  ├── Normal
  ├── Pressed
  ├── Disabled
  └── Focused</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

---

# 三十六、然后再做 Layout

第一版不要搞 AutoLayout。

首先：

<pre class="overflow-visible! px-0!" data-start="17093" data-end="17164"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">UIView_set_frame</span><span>(
    </span><span class="ͼ11">view</span><span>,
    </span><span class="ͼy">10</span><span>,
    </span><span class="ͼy">20</span><span>,
    </span><span class="ͼy">200</span><span>,
    </span><span class="ͼy">50</span><span>
);</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

等基础框架稳定，再增加：

<pre class="overflow-visible! px-0!" data-start="17180" data-end="17232"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>UIFrameLayout
UIStackLayout
UIGridLayout</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

最先做：

<pre class="overflow-visible! px-0!" data-start="17240" data-end="17283"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>Vertical Stack
Horizontal Stack</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

例如：

<pre class="overflow-visible! px-0!" data-start="17290" data-end="17531"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">UIStackView</span><span> </span><span class="ͼv">*</span><span class="ͼ11">stack</span><span> </span><span class="ͼv">=</span><span>
    </span><span class="ͼ11">UIStackView_create</span><span>(
        </span><span class="ͼ11">root</span><span>,
        </span><span class="ͼ11">UI_AXIS_VERTICAL</span><span>
    );

</span><span class="ͼ11">UIStackView_set_spacing</span><span>(
    </span><span class="ͼ11">stack</span><span>,
    </span><span class="ͼy">10</span><span>
);

</span><span class="ͼ11">UIStackView_add_view</span><span>(
    </span><span class="ͼ11">stack</span><span>,
    </span><span class="ͼ11">title</span><span>
);

</span><span class="ͼ11">UIStackView_add_view</span><span>(
    </span><span class="ͼ11">stack</span><span>,
    </span><span class="ͼ11">button</span><span>
);</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

---

# 三十七、你真正应该手搓的代码顺序

我建议严格按照这个 Git 提交历史来做。

<pre class="overflow-visible! px-0!" data-start="17581" data-end="18224"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>commit 01
init project

commit 02
add lvgl

commit 03
lvgl hello world

commit 04
UIView

commit 05
UIView frame/hidden

commit 06
UILabel

commit 07
UIButton

commit 08
UIKit event system

commit 09
UIView hierarchy

commit 10
UIView lifecycle

commit 11
UIViewController

commit 12
controller lifecycle

commit 13
UINavigationController

commit 14
navigation push/pop

commit 15
UITheme

commit 16
UILayout

commit 17
UISwitch

commit 18
UISlider

commit 19
UITextField

commit 20
UIScrollView

commit 21
animation

commit 22
memory pool

commit 23
UI thread dispatcher

commit 24
C++ wrapper

commit 25
examples

commit 26
tests</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

这样学习效果最好。

每一次 commit 都只引入一个概念。

---

# 三十八、最终代码树

等第一版完成，你的工程建议变成：

<pre class="overflow-visible! px-0!" data-start="18295" data-end="19495"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>uikit-framework/
│
├── 3rdparty/
│   └── lvgl/
│
├── config/
│   └── lv_conf.h
│
├── uikit/
│   │
│   ├── core/
│   │   ├── uikit_object.h
│   │   ├── uikit_object.c
│   │   ├── UIView.h
│   │   ├── UIView.c
│   │   ├── UIEvent.h
│   │   ├── UIEvent.c
│   │   ├── UITheme.h
│   │   └── UITheme.c
│   │
│   ├── components/
│   │   ├── UILabel.h
│   │   ├── UILabel.c
│   │   ├── UIButton.h
│   │   ├── UIButton.c
│   │   ├── UISwitch.h
│   │   ├── UISwitch.c
│   │   ├── UISlider.h
│   │   ├── UISlider.c
│   │   ├── UITextField.h
│   │   └── UITextField.c
│   │
│   ├── controller/
│   │   ├── UIViewController.h
│   │   ├── UIViewController.c
│   │   ├── UINavigationController.h
│   │   └── UINavigationController.c
│   │
│   ├── layout/
│   │   ├── UIStackView.h
│   │   ├── UIStackView.c
│   │   ├── UIGridLayout.h
│   │   └── UIGridLayout.c
│   │
│   └── cpp/
│       ├── UIView.hpp
│       ├── UIView.cpp
│       ├── UIButton.hpp
│       ├── UIButton.cpp
│       ├── UIViewController.hpp
│       └── UIViewController.cpp
│
├── examples/
│   ├── hello/
│   ├── button/
│   ├── navigation/
│   └── smart_glass/
│
├── tests/
│
├── src/
│   └── main.c
│
├── CMakeLists.txt
└── README.md</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

---

# 三十九、但我建议你再增加一个非常关键的层

你的最终架构不要直接：

<pre class="overflow-visible! px-0!" data-start="19539" data-end="19564"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>UIKit
 ↓
LVGL</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

最好是：

<pre class="overflow-visible! px-0!" data-start="19572" data-end="19676"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>Application
      │
      ▼
UIKit
      │
      ▼
UIKit Backend Adapter
      │
      ▼
LVGL</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

例如：

<pre class="overflow-visible! px-0!" data-start="19683" data-end="19975"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>UIKit
 │
 ├── UIView
 ├── UILabel
 ├── UIButton
 └── UIViewController
          │
          ▼
     uikit_lvgl/
          │
          ├── ui_view_lvgl.c
          ├── ui_label_lvgl.c
          ├── ui_button_lvgl.c
          └── ui_event_lvgl.c
          │
          ▼
         LVGL</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

这样以后理论上可以：

<pre class="overflow-visible! px-0!" data-start="19989" data-end="20087"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>UIKit
 │
 ├── LVGL Backend
 ├── SDL Backend
 ├── Custom Backend
 └── Simulator Backend</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

这会让你的 UIKit 从“LVGL 包装库”真正变成一个**UI Framework**。

---

# 四十、最终你应该达到的代码体验

底层：

<pre class="overflow-visible! px-0!" data-start="20166" data-end="20216"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">lv_obj_t</span><span> </span><span class="ͼ11">*button</span><span> </span><span class="ͼv">=</span><span> </span><span class="ͼ11">lv_btn_create</span><span>(</span><span class="ͼ11">parent</span><span>);</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

变成：

<pre class="overflow-visible! px-0!" data-start="20223" data-end="20279"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">UIButton</span><span> </span><span class="ͼv">*</span><span class="ͼ11">button</span><span> </span><span class="ͼv">=</span><span>
    </span><span class="ͼ11">UIButton_create</span><span>(</span><span class="ͼ11">parent</span><span>);</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

再变成 C++：

<pre class="overflow-visible! px-0!" data-start="20291" data-end="20335"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼv">auto</span><span> </span><span class="ͼ11">button</span><span> = UIButton::</span><span class="ͼ11">create</span><span>();</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

最终应用代码：

<pre class="overflow-visible! px-0!" data-start="20346" data-end="20878"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="relative h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class=""><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼv">class</span><span> </span><span class="ͼ11">HomeViewController</span><span>
    : </span><span class="ͼv">public</span><span> </span><span class="ͼ11">UIViewController</span><span> {

</span><span class="ͼv">public</span><span>:

    </span><span class="ͼ11">void</span><span> viewDidLoad() </span><span class="ͼv">override</span><span>
    {
        </span><span class="ͼv">auto</span><span> </span><span class="ͼ11">title</span><span> = UILabel::</span><span class="ͼ11">create</span><span>();

        </span><span class="ͼ11">title</span><span>->setText(
            </span><span class="ͼz">"Smart Glass"</span><span>
        );

        </span><span class="ͼv">auto</span><span> </span><span class="ͼ11">button</span><span> =
            UIButton::</span><span class="ͼ11">create</span><span>();

        </span><span class="ͼ11">button</span><span>->setTitle(
            </span><span class="ͼz">"Connect"</span><span>
        );

        </span><span class="ͼ11">button</span><span>->onClick(
            [</span><span class="ͼy">this</span><span>](</span><span class="ͼ11">UIButton</span><span> *) {

                </span><span class="ͼ11">connectDevice</span><span>();

            }
        );

        </span><span class="ͼ11">view</span><span>()->addSubview(</span><span class="ͼ11">title</span><span>);
        </span><span class="ͼ11">view</span><span>()->addSubview(</span><span class="ͼ11">button</span><span>);
    }
};</span></code></pre></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></div></div></pre>

这时候：

<pre class="overflow-visible! px-0!" data-start="20886" data-end="20909"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>Application</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

已经完全不需要知道：

<pre class="overflow-visible! px-0!" data-start="20923" data-end="21005"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>lv_obj_t
lv_event_t
LV_EVENT_CLICKED
lv_btn_create()
lv_label_create()</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

这才是你这个项目真正的价值。

---

# 四十一、对于你当前项目，我建议直接这样落地

结合你之前做的 **LVGL + 智能眼镜 UI + NuttX/嵌入式**，我建议不要以“通用 iOS UIKit 克隆”为目标，而是做一个：

<pre class="overflow-visible! px-0!" data-start="21127" data-end="21153"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>UIKit-Embedded</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

架构：

<pre class="overflow-visible! px-0!" data-start="21160" data-end="22053"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>                    Application
                         │
               ┌─────────┴─────────┐
               │                   │
          C++ Application       C Application
               │                   │
               └─────────┬─────────┘
                         ▼
                UIKit Framework
                         │
        ┌────────────────┼────────────────┐
        │                │                │
       View         Controller          Layout
        │                │                │
        ├────────────────┼────────────────┤
        │              Event             │
        └────────────────┼────────────────┘
                         ▼
                 LVGL Adapter
                         ▼
                       LVGL
                         ▼
              NuttX / FreeRTOS
                         ▼
                 LCD / Touch / Keys</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

特别是你的智能眼镜项目，最终可以进一步形成：

<pre class="overflow-visible! px-0!" data-start="22079" data-end="22317"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>AppCoordinator
      │
      ├── HomeVC
      ├── SettingsVC
      ├── BluetoothVC
      ├── SensorVC
      └── DeviceVC
              │
              ▼
        UIKit Framework
              │
              ▼
             LVGL</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

这样以后你写眼镜 UI 时，产品代码和 LVGL 可以基本解耦。

---

## 最关键的开发原则

**第一阶段千万不要追求“功能多”，而要追求“架构正确”。**

先做到：

<pre class="overflow-visible! px-0!" data-start="22410" data-end="22520"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>LVGL
 ↓
UIView
 ↓
UILabel
 ↓
UIButton
 ↓
Event
 ↓
View Hierarchy
 ↓
UIViewController
 ↓
Navigation</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

这 8 个东西做扎实以后，再加：

<pre class="overflow-visible! px-0!" data-start="22540" data-end="22606"><div class="relative w-full mt-4 mb-1"><div class=""><div class="contents"><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-(--code-block-surface) corner-superellipse/1.1 overflow-clip rounded-3xl [--code-block-surface:var(--bg-elevated-secondary)] dark:[--code-block-surface:var(--composer-surface-primary)] lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>Theme
Layout
Animation
C++
Memory Pool
Thread Dispatch</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></div></pre>

这样你会真正理解一个 UI Framework 是怎么从 `lv_obj_t *` 一步一步长出来的。LVGL 官方的工程组织和初始化方式本身也非常适合采用这种自底向上的学习路径
