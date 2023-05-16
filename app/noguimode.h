#ifndef NOGUIMODE_H
#define NOGUIMODE_H

/// Static behavior class for app no-GUI mode
class NoGUIMode
{
private:
    NoGUIMode();
    NoGUIMode(NoGUIMode const &);
    NoGUIMode operator=(NoGUIMode&);

public:
    /// Entering no-GUI mode
    static void Enter();
};

#endif // NOGUIMODE_H
