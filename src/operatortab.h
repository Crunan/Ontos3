#ifndef OPERATORTAB_H
#define OPERATORTAB_H

#include <QObject>

namespace Ui {
class MainWindow;
}

class OperatorTab {

public:
    OperatorTab(Ui::MainWindow* ui);

    void init();
    void hideTest();

private:
    Ui::MainWindow* m_pui;
};

#endif // OPERATORTAB_H
