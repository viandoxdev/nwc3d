#ifndef CONFIG_H
#define CONFIG_H

// Toggleable optimizations to see if they make a difference
// Toggle the optimizations that treat 2 16bit numbers as 1 32 one.
#define OPT_32 0

// Toggle if the framebuffer should be virtual: not have any real memory
// This is the only working mode as even just the framebuffer's pixel exceed the maximum ram for external apps
// So much so that two distincts arrays end up with overlapping addresses.
// In virtual mode the framebuffer is the same as the display's, push does nothing and drawned things are immediately shown.
#define FB_VIRT 1

#endif
