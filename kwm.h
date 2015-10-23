#ifndef KWM_H
#define KWM_H

#include <Carbon/Carbon.h>

#include <iostream>
#include <vector>
#include <map>
#include <string>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef DEBUG_BUILD
    #define DEBUG(x) std::cout << x << std::endl;
#else
    #define DEBUG(x)
#endif

struct window_info;
struct screen_info;
struct node_container;
struct tree_node;

enum focus_option
{ 
    FocusFollowsMouse, 
    FocusAutoraise, 
    FocusDisabled 
};

struct node_container
{    
    int X,Y;
    int Width, Height;
};

struct tree_node
{
    int WindowID;
    node_container Container;
    tree_node *Parent;
    tree_node *LeftChild;
    tree_node *RightChild;
};

struct window_info
{
    std::string Name;
    int PID, WID;
    int Layer;
    int X, Y;
    int Width, Height;
};

struct screen_info
{
    int ID;
    int X, Y;
    int Width, Height;
    int PaddingTop, PaddingBottom;
    int PaddingLeft, PaddingRight;
    std::vector<tree_node*> Space;
};

void SwapNodeWindowIDs(tree_node *A, tree_node *B);
tree_node *GetNearestNodeToTheLeft(tree_node *Node);
tree_node *GetNearestNodeToTheRight(tree_node *Node);
tree_node *GetNodeFromWindowID(tree_node *Node, int WindowID);
tree_node *CreateTreeFromWindowIDList(screen_info *Screen, std::vector<int> Windows);
void CreateLeafNodePair(screen_info *Screen, tree_node *Parent, int LeftWindowID, int RightWindowID, int SplitMode);
void SetRootNodeContainer(tree_node *Node, int X, int Y, int Width, int Height);
void ApplyNodeContainer(tree_node *Node);
void DestroyNodeTree(tree_node *Node);
tree_node *CreateLeafNode(screen_info *Screen, tree_node *Parent, int WindowID, int ContainerType);
tree_node *CreateRootNode();
node_container FullscreenContainer(screen_info *Screen, tree_node *Node);
node_container LeftVerticalContainerSplit(screen_info *Screen, tree_node *Node);
node_container UpperHorizontalContainerSplit(screen_info *Screen, tree_node *Node);
node_container LowerHorizontalContainerSplit(screen_info *Screen, tree_node *Node);
node_container RightVerticalContainerSplit(screen_info *Screen, tree_node *Node);

void GetActiveDisplays();
screen_info *GetDisplayOfWindow(window_info *Window);
std::vector<window_info*> GetAllWindowsOnDisplay(int ScreenIndex);
std::vector<int> GetAllWindowIDsOnDisplay(int ScreenIndex);
void CycleFocusedWindowDisplay(int Shift);

void CreateWindowLayout();
void RefreshWindowLayout();
void ResizeWindow(tree_node *Node);
void SetWindowDimensions(AXUIElementRef WindowRef, window_info *Window, 
                         int X, int Y, int Width, int Height);

int NumberOfSpaces();
void AddWindowToSpace(int WindowID, int SpaceIndex);
int GetSpaceOfWindow(window_info *Window);
void GetActiveSpaces();
void GetSpaceInfo(const void *Key, const void *Value, void *Context);

bool KwmHotkeyCommands(bool CmdKey, bool CtrlKey, bool AltKey, CGKeyCode Keycode);
bool SystemHotkeyPassthrough(bool CmdKey, bool CtrlKey, bool AltKey, CGKeyCode Keycode);
bool CustomHotkeyCommands(bool CmdKey, bool CtrlKey, bool AltKey, CGKeyCode Keycode);

void ToggleFocusedWindowFullscreen();
void SwapFocusedWindowWithNearest(int Shift);
void ShiftWindowFocus(int Shift);
bool WindowsAreEqual(window_info *Window, window_info *Match);
void FilterWindowList();
bool IsWindowBelowCursor(window_info *Window);
void DetectWindowBelowCursor();

void CloseWindowByRef(AXUIElementRef WindowRef);
void CloseWindow(window_info *Window);
void SetWindowRefFocus(AXUIElementRef WindowRef, window_info *Window);
void SetWindowFocus(window_info *Window);

std::string GetWindowTitle(AXUIElementRef WindowRef);
CGPoint GetWindowPos(AXUIElementRef WindowRef);
CGSize GetWindowSize(AXUIElementRef WindowRef);
window_info *GetWindowByID(int WindowID);
bool GetWindowRef(window_info *Window, AXUIElementRef *WindowRef);
void GetWindowInfo(const void *Key, const void *Value, void *Context);

CGEventRef CGEventCallback(CGEventTapProxy Proxy, CGEventType Type, CGEventRef Event, void *Refcon);
bool CheckPrivileges();
void KwmRestart();
void Fatal(const std::string &Err);

#endif
