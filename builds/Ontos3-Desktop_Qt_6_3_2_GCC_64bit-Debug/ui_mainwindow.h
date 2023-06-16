/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionConnect;
    QAction *actionDisconnect;
    QAction *actionQuit;
    QAction *actionConfigure;
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *Dashboard;
    QWidget *layoutWidget;
    QGridLayout *recipeLayout;
    QGridLayout *gridLayout_21;
    QLabel *label_32;
    QTextEdit *mfc3_recipe;
    QPushButton *load_MFC4;
    QPushButton *load_MFC3;
    QTextEdit *mfc2_recipe;
    QTextEdit *mfc1_recipe;
    QTextEdit *mfc4_recipe;
    QPushButton *load_MFC2;
    QPushButton *load_MFC1;
    QGridLayout *gridLayout_17;
    QTextEdit *RF_recipe;
    QTextEdit *mb_recipe;
    QLabel *label_49;
    QPushButton *autotune_recipe;
    QPushButton *load_MB;
    QPushButton *load_autotune;
    QPushButton *load_RF;
    QLabel *label_87;
    QLabel *label_103;
    QGridLayout *gridLayout_20;
    QLabel *label_89;
    QTextEdit *cycles_recipe;
    QLabel *label_100;
    QTextEdit *Z_relative_PH;
    QPushButton *load_cycles;
    QTextEdit *x1_recipe;
    QPushButton *load_gap;
    QPushButton *load_speed;
    QTextEdit *y1_recipe;
    QPushButton *Y1_set;
    QLabel *label_88;
    QPushButton *load_overlap;
    QPushButton *load_thick;
    QTextEdit *X_relative_PH;
    QTextEdit *overlap_recipe;
    QTextEdit *gap_recipe;
    QPushButton *x1_set;
    QPushButton *Y2_set;
    QLabel *label_51;
    QTextEdit *speed_recipe;
    QTextEdit *Y_relative_PH;
    QTextEdit *thickness_recipe;
    QPushButton *load_autoscan;
    QPushButton *x2_set;
    QLabel *label_102;
    QLabel *label_50;
    QLabel *label_95;
    QTextEdit *x2_recipe;
    QPushButton *autoscan_recipe;
    QTextEdit *y2_recipe;
    QPushButton *save_recipe_button;
    QLabel *label_31;
    QPushButton *load_recipe_button;
    QTextEdit *name_recipe;
    QWidget *layoutWidget1;
    QHBoxLayout *Power_Layout_2;
    QVBoxLayout *verticalLayout_18;
    QHBoxLayout *horizontalLayout_18;
    QLCDNumber *RF_Actual_LCD;
    QLabel *RFfor_label_2;
    QProgressBar *RF_bar;
    QProgressBar *RefRF_bar;
    QHBoxLayout *horizontalLayout_20;
    QLabel *ref_label_3;
    QLCDNumber *RefRF_Actual_LCD;
    QHBoxLayout *horizontalLayout_19;
    QLCDNumber *MB_Actual_LCD;
    QLabel *MBfor_label_3;
    QProgressBar *MB_Pos_Bar;
    QHBoxLayout *horizontalLayout_13;
    QPushButton *MB_Left_Button;
    QVBoxLayout *verticalLayout_15;
    QLabel *stepLabel;
    QComboBox *stepSizeBox;
    QPushButton *MB_Right_Button;
    QHBoxLayout *horizontalLayout_22;
    QLabel *ref_label_4;
    QLCDNumber *temp_LCD;
    QProgressBar *Temp_bar;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *gasses;
    QHBoxLayout *Gasses_Layout_2;
    QVBoxLayout *Gas1_Layout_2;
    QLabel *gas1_label;
    QHBoxLayout *gas1_progress_slider_Layout_2;
    QSlider *gas1_sliderbar;
    QProgressBar *gas1_progressbar;
    QHBoxLayout *gas1_button_readout_layout_2;
    QVBoxLayout *gas1_recipe_actual_SLPM_2;
    QLabel *gas1_actual_SLPM;
    QLabel *gas1_recipe_SLPM;
    QVBoxLayout *Gas2_Layout_2;
    QLabel *gas2_label;
    QHBoxLayout *gas2_progress_slider_layout_2;
    QSlider *gas2_sliderbar;
    QProgressBar *gas2_progressbar;
    QHBoxLayout *gas2_buttton_readout_layout_2;
    QVBoxLayout *gas2_recipe_actual_SLPM_2;
    QLabel *gas2_actual_SLPM;
    QLabel *gas2_recipe_SLPM;
    QVBoxLayout *Gas3_Layout_2;
    QLabel *gas3_label;
    QHBoxLayout *horizontalLayout_25;
    QSlider *gas3_sliderbar;
    QProgressBar *gas3_progessbar;
    QHBoxLayout *horizontalLayout_16;
    QVBoxLayout *verticalLayout_16;
    QLabel *gas3_actual_SLPM;
    QLabel *gas3_recipe_SLPM;
    QVBoxLayout *Gas4_Layout_2;
    QLabel *gas4_label;
    QHBoxLayout *horizontalLayout_27;
    QSlider *gas4_sliderbar;
    QProgressBar *gas4_progressbar;
    QHBoxLayout *horizontalLayout_17;
    QVBoxLayout *verticalLayout_17;
    QLabel *gas4_actual_SLPM;
    QLabel *gas4_recipe_SLPM;
    QPushButton *plsmaBtn;
    QWidget *layoutWidget2;
    QVBoxLayout *verticalLayout_21;
    QLabel *label_52;
    QTextEdit *axisstatus;
    QLabel *label_53;
    QTextEdit *axisstatus_2;
    QWidget *gridLayoutWidget_7;
    QGridLayout *gridLayout_8;
    QPushButton *vac_button;
    QPushButton *twospot_button;
    QPushButton *init_button;
    QPushButton *Joystick_button;
    QVBoxLayout *verticalLayout_22;
    QLabel *label_55;
    QComboBox *wafer_diameter;
    QLabel *label_54;
    QPushButton *scan_button;
    QPushButton *diameter_button;
    QPushButton *Home_button;
    QPushButton *Stagepins_button;
    QPushButton *n2_purge_button;
    QWidget *recipe;
    QGroupBox *groupBox;
    QWidget *gridLayoutWidget_13;
    QGridLayout *gridLayout_10;
    QPushButton *remove_cascade_recipe_button;
    QPushButton *add_cascade_recipe_button;
    QPushButton *clear_cascade_recipe_button;
    QListWidget *cascadeList;
    QTextEdit *cascade_recipe_name;
    QPushButton *load_cascade_recipe_button;
    QPushButton *save_cascade_recipe_button;
    QWidget *plasma;
    QWidget *gridLayoutWidget_3;
    QGridLayout *gridLayout_3;
    QHBoxLayout *matchbox_layout;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *MB_Layout;
    QHBoxLayout *horizontalLayout_11;
    QHBoxLayout *horizontalLayout_6;
    QHBoxLayout *Gasses_Layout;
    QVBoxLayout *Gas1_Layout;
    QLabel *gas1_label_dup;
    QHBoxLayout *gas1_progress_slider_Layout;
    QSlider *gas1_sliderbar_dup;
    QProgressBar *progressBar_dup;
    QHBoxLayout *gas1_button_readout_layout;
    QPushButton *gas1_setpoint_button_dup;
    QVBoxLayout *gas1_recipe_actual_SLPM;
    QLabel *gas1_actual_SLPM_dup;
    QLabel *gas1_recipe_SLPM_dup;
    QVBoxLayout *Gas2_Layout;
    QLabel *gas2_label_dup;
    QHBoxLayout *gas2_progress_slider_layout;
    QSlider *gas2_sliderbar_dup;
    QProgressBar *gas2_progressbar_dup;
    QHBoxLayout *gas2_buttton_readout_layout;
    QPushButton *gas2_setpoint_button_dup;
    QVBoxLayout *gas2_recipe_actual_SLPM;
    QLabel *gas2_actual_SLPM_dup;
    QLabel *gas2_recipe_SLPM_dup;
    QVBoxLayout *Gas3_Layout;
    QLabel *gas3_label_dup;
    QHBoxLayout *horizontalLayout_23;
    QSlider *gas3_sliderbar_dup;
    QProgressBar *gas3_progessbar_dup;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *gas3_setpoint_button_dup;
    QVBoxLayout *verticalLayout_6;
    QLabel *gas3_actual_SLPM_dup;
    QLabel *gas3_recipe_SLPM_dup;
    QVBoxLayout *Gas4_Layout;
    QLabel *gas4_label_dup;
    QHBoxLayout *horizontalLayout_24;
    QSlider *gas4_sliderbar_dup;
    QProgressBar *gas4_progressbar_dup;
    QHBoxLayout *horizontalLayout_9;
    QPushButton *gas4_setpoint_button_dup;
    QVBoxLayout *verticalLayout_8;
    QLabel *gas4_actual_SLPM_dup;
    QLabel *gas4_recipe_SLPM_dup;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *Power_Layout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_4;
    QLCDNumber *RF_Actual_LCD_dup;
    QLabel *RFfor_label;
    QProgressBar *RF_bar_dup;
    QHBoxLayout *horizontalLayout_7;
    QLCDNumber *MB_Actual_LCD_dup;
    QLabel *MBfor_label_2;
    QProgressBar *MB_Pos_Bar_dup;
    QHBoxLayout *horizontalLayout_10;
    QLCDNumber *RefRF_Actual_LCD_dup;
    QLabel *ref_label;
    QProgressBar *RefRF_bar_dup;
    QVBoxLayout *RF_Dials;
    QHBoxLayout *horizontalLayout_15;
    QVBoxLayout *verticalLayout_9;
    QHBoxLayout *horizontalLayout_12;
    QLCDNumber *temp_LCD_dup;
    QLabel *label_20;
    QLabel *ref_label_2;
    QProgressBar *Temp_bar_dup;
    QHBoxLayout *horizontalLayout_26;
    QVBoxLayout *verticalLayout_13;
    QPushButton *RF_Setpoint_Button;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *autoMode_Button_dup;
    QPushButton *MB_Setpoint_Button;
    QHBoxLayout *horizontalLayout;
    QPushButton *MB_Left_Button_dup;
    QPushButton *MB_Right_Button_dup;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *Heater_Button;
    QSpacerItem *verticalSpacer;
    QPushButton *plsmaBtn_dup;
    QSpacerItem *verticalSpacer_3;
    QLabel *Gasses_Label;
    QPushButton *Send_CMD_button;
    QPlainTextEdit *textRCVbox;
    QPushButton *Clear_Button;
    QLabel *CMDlabel;
    QPlainTextEdit *text2sendCMDbox;
    QLabel *RCVlabel;
    QPlainTextEdit *textCMDbox;
    QWidget *axis;
    QLabel *label_39;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QPushButton *init_button_dup;
    QPushButton *Stagepins_button_dup;
    QLabel *label_45;
    QPushButton *diameter_button_dup;
    QPushButton *Home_button_dup;
    QPushButton *twospot_button_dup;
    QPushButton *scan_button_dup;
    QPushButton *vac_button_dup;
    QPushButton *n2_purge_button_dup;
    QVBoxLayout *verticalLayout_14;
    QLabel *label_33;
    QComboBox *wafer_diameter_dup;
    QPushButton *Joystick_button_dup;
    QWidget *verticalLayoutWidget_7;
    QVBoxLayout *verticalLayout_10;
    QLabel *label_34;
    QTextEdit *X_relative_PH_dup;
    QLabel *label_84;
    QTextEdit *Y_relative_PH_dup;
    QLabel *label_85;
    QTextEdit *Z_relative_PH_dup;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_43;
    QTextEdit *input_thickness_dup;
    QLabel *label_42;
    QTextEdit *input_gap_dup;
    QLabel *label_38;
    QTextEdit *input_overlap_dup;
    QLabel *label_40;
    QTextEdit *input_speed_dup;
    QLabel *label_41;
    QTextEdit *input_cycles_dup;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QTextEdit *xmin_controls_dup;
    QTextEdit *ymax_controls_dup;
    QLabel *label_47;
    QTextEdit *ymin_controls_dup;
    QTextEdit *xmax_controls_dup;
    QLabel *label_35;
    QLabel *label_44;
    QLabel *label_48;
    QLabel *label_46;
    QLabel *label_37;
    QWidget *gridLayoutWidget_5;
    QGridLayout *gridLayout_5;
    QVBoxLayout *verticalLayout_12;
    QLabel *label_24;
    QTextEdit *axisstatus_dup;
    QLabel *label_25;
    QTextEdit *axisstatus_2_dup;
    QPushButton *door2;
    QPushButton *door1;
    QWidget *settings;
    QGroupBox *groupBox_2;
    QWidget *gridLayoutWidget_4;
    QGridLayout *gridLayout_4;
    QCheckBox *heater_checkbox;
    QCheckBox *ctldiagnostics_checkbox;
    QCheckBox *mfc_loaded_checkbox;
    QCheckBox *batchID_checkBox;
    QCheckBox *request_terminal;
    QWidget *tab;
    QWidget *tab_2;
    QGroupBox *groupBox_5;
    QWidget *gridLayoutWidget_15;
    QGridLayout *gridLayout_31;
    QSpacerItem *horizontalSpacer_38;
    QGridLayout *gridLayout_32;
    QLabel *label_115;
    QPushButton *autotune_recipe_dup_3;
    QPushButton *load_RF_dup_3;
    QPushButton *load_MB_dup_3;
    QTextEdit *mb_recipe_dup_3;
    QTextEdit *RF_recipe_dup_3;
    QLabel *label_116;
    QPushButton *load_autotune_dup_3;
    QSpacerItem *horizontalSpacer_39;
    QGridLayout *gridLayout_33;
    QGridLayout *gridLayout_34;
    QSpacerItem *horizontalSpacer_40;
    QSpacerItem *horizontalSpacer_41;
    QSpacerItem *horizontalSpacer_42;
    QSpacerItem *horizontalSpacer_43;
    QSpacerItem *horizontalSpacer_44;
    QSpacerItem *horizontalSpacer_45;
    QSpacerItem *horizontalSpacer_46;
    QLabel *label_117;
    QSpacerItem *horizontalSpacer_47;
    QSpacerItem *horizontalSpacer_48;
    QSpacerItem *horizontalSpacer_49;
    QTextEdit *cycles_recipe_dup_3;
    QSpacerItem *horizontalSpacer_50;
    QSpacerItem *horizontalSpacer_51;
    QSpacerItem *horizontalSpacer_52;
    QTextEdit *speed_recipe_dup_3;
    QPushButton *autoscan_recipe_dup_3;
    QSpacerItem *horizontalSpacer_53;
    QPushButton *load_autoscan_dup_3;
    QPushButton *load_cycles_dup_3;
    QPushButton *load_speed_dup_3;
    QGridLayout *gridLayout_35;
    QTextEdit *ymin_recipe_dup_3;
    QPushButton *load_thick_dup_3;
    QTextEdit *xmax_recipe_dup_3;
    QLabel *label_118;
    QPushButton *x1_set_dup_3;
    QTextEdit *xmin_recipe_dup_3;
    QTextEdit *ymax_recipe_dup_3;
    QTextEdit *gap_recipe_dup_3;
    QTextEdit *overlap_recipe_dup_3;
    QPushButton *Y1_set_dup_3;
    QTextEdit *thickness_recipe_dup_3;
    QPushButton *load_overlap_dup_3;
    QPushButton *load_gap_dup_3;
    QPushButton *x2_set_dup_3;
    QPushButton *Y2_set_dup_3;
    QLabel *label_119;
    QLabel *label_120;
    QLabel *label_121;
    QTextEdit *name_recipe_dup_3;
    QLabel *label_122;
    QPushButton *save_recipe_button_dup_3;
    QPushButton *load_recipe_button_dup_3;
    QGridLayout *gridLayout_36;
    QTextEdit *mfc1_recipe_dup_3;
    QTextEdit *mfc4_recipe_dup_3;
    QTextEdit *mfc2_recipe_dup_3;
    QTextEdit *mfc3_recipe_dup_3;
    QLabel *label_123;
    QPushButton *load_MFC1_dup_3;
    QPushButton *load_MFC2_dup_3;
    QPushButton *load_MFC4_dup_3;
    QPushButton *load_MFC3_dup_3;
    QToolBar *toolBar_2;
    QStatusBar *statusBar;
    QToolBar *toolBar_3;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1960, 1015);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(75, 75));
        MainWindow->setStyleSheet(QString::fromUtf8("QWidget \n"
"{\n"
"font-family:  \"Century Gothic\";\n"
"}\n"
"\n"
"QPushButton, QGroupBox, QRadioButton, QComboBpx, QSlider, QLCDNumber, QCheckBox, QSpinBox, QLabel, QProgressBar, QLineEdit, QWebView, QTextEdit, QDoubleSpinBox\n"
"{\n"
"color: black;\n"
"}\n"
"QLabel\n"
"{\n"
"color:black;\n"
"}\n"
"QProgressBar \n"
"{		\n"
"	border-radius: 26px;\n"
"	background-color: #d3dbdd;\n"
"}\n"
"\n"
"QProgressBar::Chunk\n"
"{\n"
"    border-radius: 26px;\n"
"	background-color:  #c92a26;\n"
"}\n"
"\n"
""));
        actionConnect = new QAction(MainWindow);
        actionConnect->setObjectName(QString::fromUtf8("actionConnect"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/connect.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionConnect->setIcon(icon);
        actionDisconnect = new QAction(MainWindow);
        actionDisconnect->setObjectName(QString::fromUtf8("actionDisconnect"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/disconnect.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDisconnect->setIcon(icon1);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        QIcon icon2;
        QString iconThemeName = QString::fromUtf8("system-shutdown");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon2 = QIcon::fromTheme(iconThemeName);
        } else {
            icon2.addFile(QString::fromUtf8(":/images/application-exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        }
        actionQuit->setIcon(icon2);
        actionConfigure = new QAction(MainWindow);
        actionConfigure->setObjectName(QString::fromUtf8("actionConfigure"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionConfigure->setIcon(icon3);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(60, 10, 1611, 921));
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setIconSize(QSize(32, 50));
        Dashboard = new QWidget();
        Dashboard->setObjectName(QString::fromUtf8("Dashboard"));
        layoutWidget = new QWidget(Dashboard);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(960, 30, 406, 771));
        recipeLayout = new QGridLayout(layoutWidget);
        recipeLayout->setObjectName(QString::fromUtf8("recipeLayout"));
        recipeLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout_21 = new QGridLayout();
        gridLayout_21->setObjectName(QString::fromUtf8("gridLayout_21"));
        label_32 = new QLabel(layoutWidget);
        label_32->setObjectName(QString::fromUtf8("label_32"));
        label_32->setStyleSheet(QString::fromUtf8("\n"
"font: 700 14pt \"Ubuntu Mono\";"));

        gridLayout_21->addWidget(label_32, 0, 0, 1, 2);

        mfc3_recipe = new QTextEdit(layoutWidget);
        mfc3_recipe->setObjectName(QString::fromUtf8("mfc3_recipe"));
        mfc3_recipe->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(15);
        sizePolicy1.setVerticalStretch(20);
        sizePolicy1.setHeightForWidth(mfc3_recipe->sizePolicy().hasHeightForWidth());
        mfc3_recipe->setSizePolicy(sizePolicy1);
        mfc3_recipe->setMinimumSize(QSize(15, 15));
        mfc3_recipe->setStyleSheet(QString::fromUtf8("font: 18px;"));
        mfc3_recipe->setReadOnly(true);

        gridLayout_21->addWidget(mfc3_recipe, 3, 1, 1, 1);

        load_MFC4 = new QPushButton(layoutWidget);
        load_MFC4->setObjectName(QString::fromUtf8("load_MFC4"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(load_MFC4->sizePolicy().hasHeightForWidth());
        load_MFC4->setSizePolicy(sizePolicy2);
        load_MFC4->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 16pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_21->addWidget(load_MFC4, 4, 0, 1, 1);

        load_MFC3 = new QPushButton(layoutWidget);
        load_MFC3->setObjectName(QString::fromUtf8("load_MFC3"));
        sizePolicy2.setHeightForWidth(load_MFC3->sizePolicy().hasHeightForWidth());
        load_MFC3->setSizePolicy(sizePolicy2);
        load_MFC3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 16pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_21->addWidget(load_MFC3, 3, 0, 1, 1);

        mfc2_recipe = new QTextEdit(layoutWidget);
        mfc2_recipe->setObjectName(QString::fromUtf8("mfc2_recipe"));
        mfc2_recipe->setEnabled(true);
        sizePolicy1.setHeightForWidth(mfc2_recipe->sizePolicy().hasHeightForWidth());
        mfc2_recipe->setSizePolicy(sizePolicy1);
        mfc2_recipe->setMinimumSize(QSize(15, 15));
        mfc2_recipe->setStyleSheet(QString::fromUtf8("font: 18px;"));
        mfc2_recipe->setReadOnly(true);

        gridLayout_21->addWidget(mfc2_recipe, 2, 1, 1, 1);

        mfc1_recipe = new QTextEdit(layoutWidget);
        mfc1_recipe->setObjectName(QString::fromUtf8("mfc1_recipe"));
        mfc1_recipe->setEnabled(true);
        sizePolicy1.setHeightForWidth(mfc1_recipe->sizePolicy().hasHeightForWidth());
        mfc1_recipe->setSizePolicy(sizePolicy1);
        mfc1_recipe->setMinimumSize(QSize(15, 15));
        mfc1_recipe->setStyleSheet(QString::fromUtf8("font: 18px;"));
        mfc1_recipe->setReadOnly(true);

        gridLayout_21->addWidget(mfc1_recipe, 1, 1, 1, 1);

        mfc4_recipe = new QTextEdit(layoutWidget);
        mfc4_recipe->setObjectName(QString::fromUtf8("mfc4_recipe"));
        mfc4_recipe->setEnabled(true);
        sizePolicy1.setHeightForWidth(mfc4_recipe->sizePolicy().hasHeightForWidth());
        mfc4_recipe->setSizePolicy(sizePolicy1);
        mfc4_recipe->setMinimumSize(QSize(15, 15));
        mfc4_recipe->setStyleSheet(QString::fromUtf8("font: 18px;"));
        mfc4_recipe->setReadOnly(true);

        gridLayout_21->addWidget(mfc4_recipe, 4, 1, 1, 1);

        load_MFC2 = new QPushButton(layoutWidget);
        load_MFC2->setObjectName(QString::fromUtf8("load_MFC2"));
        sizePolicy2.setHeightForWidth(load_MFC2->sizePolicy().hasHeightForWidth());
        load_MFC2->setSizePolicy(sizePolicy2);
        load_MFC2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 16pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_21->addWidget(load_MFC2, 2, 0, 1, 1);

        load_MFC1 = new QPushButton(layoutWidget);
        load_MFC1->setObjectName(QString::fromUtf8("load_MFC1"));
        sizePolicy2.setHeightForWidth(load_MFC1->sizePolicy().hasHeightForWidth());
        load_MFC1->setSizePolicy(sizePolicy2);
        load_MFC1->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 16pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_21->addWidget(load_MFC1, 1, 0, 1, 1);


        recipeLayout->addLayout(gridLayout_21, 5, 0, 1, 1);

        gridLayout_17 = new QGridLayout();
        gridLayout_17->setObjectName(QString::fromUtf8("gridLayout_17"));
        RF_recipe = new QTextEdit(layoutWidget);
        RF_recipe->setObjectName(QString::fromUtf8("RF_recipe"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(15);
        sizePolicy3.setHeightForWidth(RF_recipe->sizePolicy().hasHeightForWidth());
        RF_recipe->setSizePolicy(sizePolicy3);
        RF_recipe->setMinimumSize(QSize(0, 15));
        RF_recipe->setStyleSheet(QString::fromUtf8("font: 18px;\n"
"color: rgb(52, 101, 164);"));
        RF_recipe->setReadOnly(true);

        gridLayout_17->addWidget(RF_recipe, 1, 1, 1, 1);

        mb_recipe = new QTextEdit(layoutWidget);
        mb_recipe->setObjectName(QString::fromUtf8("mb_recipe"));
        sizePolicy3.setHeightForWidth(mb_recipe->sizePolicy().hasHeightForWidth());
        mb_recipe->setSizePolicy(sizePolicy3);
        mb_recipe->setMinimumSize(QSize(0, 15));
        mb_recipe->setStyleSheet(QString::fromUtf8("font: 18px;\n"
"color: rgb(52, 101, 164);"));
        mb_recipe->setReadOnly(true);

        gridLayout_17->addWidget(mb_recipe, 3, 1, 1, 1);

        label_49 = new QLabel(layoutWidget);
        label_49->setObjectName(QString::fromUtf8("label_49"));
        label_49->setStyleSheet(QString::fromUtf8("\n"
"font: 700 14pt \"Ubuntu Mono\";"));

        gridLayout_17->addWidget(label_49, 0, 0, 1, 2);

        autotune_recipe = new QPushButton(layoutWidget);
        autotune_recipe->setObjectName(QString::fromUtf8("autotune_recipe"));
        autotune_recipe->setMinimumSize(QSize(0, 20));
        autotune_recipe->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"font: 16pt \"Ubuntu Bold\";\n"
"	color: rgb(220, 0, 20);\n"
"}"));
        autotune_recipe->setIconSize(QSize(32, 32));

        gridLayout_17->addWidget(autotune_recipe, 4, 1, 1, 1);

        load_MB = new QPushButton(layoutWidget);
        load_MB->setObjectName(QString::fromUtf8("load_MB"));
        sizePolicy2.setHeightForWidth(load_MB->sizePolicy().hasHeightForWidth());
        load_MB->setSizePolicy(sizePolicy2);
        load_MB->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 14pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_17->addWidget(load_MB, 3, 0, 1, 1);

        load_autotune = new QPushButton(layoutWidget);
        load_autotune->setObjectName(QString::fromUtf8("load_autotune"));
        sizePolicy.setHeightForWidth(load_autotune->sizePolicy().hasHeightForWidth());
        load_autotune->setSizePolicy(sizePolicy);
        load_autotune->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 10pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_17->addWidget(load_autotune, 4, 0, 1, 1);

        load_RF = new QPushButton(layoutWidget);
        load_RF->setObjectName(QString::fromUtf8("load_RF"));
        sizePolicy2.setHeightForWidth(load_RF->sizePolicy().hasHeightForWidth());
        load_RF->setSizePolicy(sizePolicy2);
        load_RF->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 16pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_17->addWidget(load_RF, 1, 0, 1, 1);

        label_87 = new QLabel(layoutWidget);
        label_87->setObjectName(QString::fromUtf8("label_87"));
        label_87->setStyleSheet(QString::fromUtf8("\n"
"font: 700 14pt \"Ubuntu Mono\";"));

        gridLayout_17->addWidget(label_87, 2, 0, 1, 2);


        recipeLayout->addLayout(gridLayout_17, 5, 1, 1, 2);

        label_103 = new QLabel(layoutWidget);
        label_103->setObjectName(QString::fromUtf8("label_103"));
        label_103->setStyleSheet(QString::fromUtf8("font: 18px;"));

        recipeLayout->addWidget(label_103, 4, 0, 1, 3);

        gridLayout_20 = new QGridLayout();
        gridLayout_20->setObjectName(QString::fromUtf8("gridLayout_20"));
        label_89 = new QLabel(layoutWidget);
        label_89->setObjectName(QString::fromUtf8("label_89"));
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(15);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(label_89->sizePolicy().hasHeightForWidth());
        label_89->setSizePolicy(sizePolicy4);
        label_89->setMinimumSize(QSize(15, 0));
        QFont font;
        font.setFamilies({QString::fromUtf8("Century Gothic")});
        font.setPointSize(16);
        label_89->setFont(font);
        label_89->setAlignment(Qt::AlignCenter);

        gridLayout_20->addWidget(label_89, 11, 4, 1, 1);

        cycles_recipe = new QTextEdit(layoutWidget);
        cycles_recipe->setObjectName(QString::fromUtf8("cycles_recipe"));
        sizePolicy3.setHeightForWidth(cycles_recipe->sizePolicy().hasHeightForWidth());
        cycles_recipe->setSizePolicy(sizePolicy3);
        cycles_recipe->setMinimumSize(QSize(0, 15));
        cycles_recipe->setStyleSheet(QString::fromUtf8("font: 18px;\n"
"color: rgb(52, 101, 164);"));
        cycles_recipe->setReadOnly(true);

        gridLayout_20->addWidget(cycles_recipe, 2, 5, 1, 1);

        label_100 = new QLabel(layoutWidget);
        label_100->setObjectName(QString::fromUtf8("label_100"));
        QSizePolicy sizePolicy5(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(label_100->sizePolicy().hasHeightForWidth());
        label_100->setSizePolicy(sizePolicy5);
        label_100->setStyleSheet(QString::fromUtf8("\n"
"font: 700 16pt \"Ubuntu Mono\";"));

        gridLayout_20->addWidget(label_100, 0, 0, 1, 4);

        Z_relative_PH = new QTextEdit(layoutWidget);
        Z_relative_PH->setObjectName(QString::fromUtf8("Z_relative_PH"));
        Z_relative_PH->setEnabled(true);
        QSizePolicy sizePolicy6(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy6.setHorizontalStretch(15);
        sizePolicy6.setVerticalStretch(50);
        sizePolicy6.setHeightForWidth(Z_relative_PH->sizePolicy().hasHeightForWidth());
        Z_relative_PH->setSizePolicy(sizePolicy6);
        Z_relative_PH->setMinimumSize(QSize(15, 50));
        Z_relative_PH->setStyleSheet(QString::fromUtf8("font: 18px;\n"
"color: rgb(52, 101, 164);"));
        Z_relative_PH->setReadOnly(true);

        gridLayout_20->addWidget(Z_relative_PH, 11, 5, 1, 1);

        load_cycles = new QPushButton(layoutWidget);
        load_cycles->setObjectName(QString::fromUtf8("load_cycles"));
        sizePolicy2.setHeightForWidth(load_cycles->sizePolicy().hasHeightForWidth());
        load_cycles->setSizePolicy(sizePolicy2);
        load_cycles->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 12pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_20->addWidget(load_cycles, 2, 4, 1, 1);

        x1_recipe = new QTextEdit(layoutWidget);
        x1_recipe->setObjectName(QString::fromUtf8("x1_recipe"));
        QSizePolicy sizePolicy7(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy7.setHorizontalStretch(15);
        sizePolicy7.setVerticalStretch(15);
        sizePolicy7.setHeightForWidth(x1_recipe->sizePolicy().hasHeightForWidth());
        x1_recipe->setSizePolicy(sizePolicy7);
        x1_recipe->setMinimumSize(QSize(15, 15));
        x1_recipe->setStyleSheet(QString::fromUtf8("font: 18px;\n"
"color: rgb(52, 101, 164);"));
        x1_recipe->setReadOnly(true);

        gridLayout_20->addWidget(x1_recipe, 9, 1, 1, 3);

        load_gap = new QPushButton(layoutWidget);
        load_gap->setObjectName(QString::fromUtf8("load_gap"));
        sizePolicy2.setHeightForWidth(load_gap->sizePolicy().hasHeightForWidth());
        load_gap->setSizePolicy(sizePolicy2);
        load_gap->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 14pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_20->addWidget(load_gap, 2, 0, 1, 1);

        load_speed = new QPushButton(layoutWidget);
        load_speed->setObjectName(QString::fromUtf8("load_speed"));
        sizePolicy2.setHeightForWidth(load_speed->sizePolicy().hasHeightForWidth());
        load_speed->setSizePolicy(sizePolicy2);
        load_speed->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 14pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_20->addWidget(load_speed, 1, 4, 1, 1);

        y1_recipe = new QTextEdit(layoutWidget);
        y1_recipe->setObjectName(QString::fromUtf8("y1_recipe"));
        sizePolicy7.setHeightForWidth(y1_recipe->sizePolicy().hasHeightForWidth());
        y1_recipe->setSizePolicy(sizePolicy7);
        y1_recipe->setMinimumSize(QSize(15, 15));
        y1_recipe->setStyleSheet(QString::fromUtf8("font: 18px;\n"
"color: rgb(52, 101, 164);"));
        y1_recipe->setReadOnly(true);

        gridLayout_20->addWidget(y1_recipe, 11, 1, 1, 3);

        Y1_set = new QPushButton(layoutWidget);
        Y1_set->setObjectName(QString::fromUtf8("Y1_set"));
        sizePolicy2.setHeightForWidth(Y1_set->sizePolicy().hasHeightForWidth());
        Y1_set->setSizePolicy(sizePolicy2);
        Y1_set->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_20->addWidget(Y1_set, 11, 0, 1, 1);

        label_88 = new QLabel(layoutWidget);
        label_88->setObjectName(QString::fromUtf8("label_88"));
        sizePolicy4.setHeightForWidth(label_88->sizePolicy().hasHeightForWidth());
        label_88->setSizePolicy(sizePolicy4);
        label_88->setMinimumSize(QSize(15, 0));
        label_88->setFont(font);
        label_88->setAlignment(Qt::AlignCenter);

        gridLayout_20->addWidget(label_88, 10, 4, 1, 1);

        load_overlap = new QPushButton(layoutWidget);
        load_overlap->setObjectName(QString::fromUtf8("load_overlap"));
        sizePolicy2.setHeightForWidth(load_overlap->sizePolicy().hasHeightForWidth());
        load_overlap->setSizePolicy(sizePolicy2);
        load_overlap->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 10pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_20->addWidget(load_overlap, 3, 0, 1, 1);

        load_thick = new QPushButton(layoutWidget);
        load_thick->setObjectName(QString::fromUtf8("load_thick"));
        sizePolicy2.setHeightForWidth(load_thick->sizePolicy().hasHeightForWidth());
        load_thick->setSizePolicy(sizePolicy2);
        load_thick->setMinimumSize(QSize(35, 0));
        load_thick->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 8pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_20->addWidget(load_thick, 1, 0, 1, 1);

        X_relative_PH = new QTextEdit(layoutWidget);
        X_relative_PH->setObjectName(QString::fromUtf8("X_relative_PH"));
        X_relative_PH->setEnabled(true);
        sizePolicy6.setHeightForWidth(X_relative_PH->sizePolicy().hasHeightForWidth());
        X_relative_PH->setSizePolicy(sizePolicy6);
        X_relative_PH->setMinimumSize(QSize(15, 50));
        X_relative_PH->setStyleSheet(QString::fromUtf8("font: 18px;\n"
"color: rgb(52, 101, 164);"));
        X_relative_PH->setReadOnly(true);

        gridLayout_20->addWidget(X_relative_PH, 9, 5, 1, 1);

        overlap_recipe = new QTextEdit(layoutWidget);
        overlap_recipe->setObjectName(QString::fromUtf8("overlap_recipe"));
        QSizePolicy sizePolicy8(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        sizePolicy8.setHorizontalStretch(15);
        sizePolicy8.setVerticalStretch(15);
        sizePolicy8.setHeightForWidth(overlap_recipe->sizePolicy().hasHeightForWidth());
        overlap_recipe->setSizePolicy(sizePolicy8);
        overlap_recipe->setMinimumSize(QSize(15, 15));
        overlap_recipe->setStyleSheet(QString::fromUtf8("font: 18px;\n"
"color: rgb(52, 101, 164);"));
        overlap_recipe->setReadOnly(true);

        gridLayout_20->addWidget(overlap_recipe, 3, 1, 1, 3);

        gap_recipe = new QTextEdit(layoutWidget);
        gap_recipe->setObjectName(QString::fromUtf8("gap_recipe"));
        sizePolicy8.setHeightForWidth(gap_recipe->sizePolicy().hasHeightForWidth());
        gap_recipe->setSizePolicy(sizePolicy8);
        gap_recipe->setMinimumSize(QSize(15, 15));
        gap_recipe->setStyleSheet(QString::fromUtf8("font: 18px;\n"
"color: rgb(52, 101, 164);"));
        gap_recipe->setReadOnly(true);

        gridLayout_20->addWidget(gap_recipe, 2, 1, 1, 3);

        x1_set = new QPushButton(layoutWidget);
        x1_set->setObjectName(QString::fromUtf8("x1_set"));
        sizePolicy2.setHeightForWidth(x1_set->sizePolicy().hasHeightForWidth());
        x1_set->setSizePolicy(sizePolicy2);
        x1_set->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_20->addWidget(x1_set, 9, 0, 1, 1);

        Y2_set = new QPushButton(layoutWidget);
        Y2_set->setObjectName(QString::fromUtf8("Y2_set"));
        sizePolicy2.setHeightForWidth(Y2_set->sizePolicy().hasHeightForWidth());
        Y2_set->setSizePolicy(sizePolicy2);
        Y2_set->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_20->addWidget(Y2_set, 12, 0, 1, 1);

        label_51 = new QLabel(layoutWidget);
        label_51->setObjectName(QString::fromUtf8("label_51"));
        QSizePolicy sizePolicy9(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy9.setHorizontalStretch(15);
        sizePolicy9.setVerticalStretch(15);
        sizePolicy9.setHeightForWidth(label_51->sizePolicy().hasHeightForWidth());
        label_51->setSizePolicy(sizePolicy9);
        label_51->setMinimumSize(QSize(15, 15));
        label_51->setFont(font);
        label_51->setAlignment(Qt::AlignCenter);

        gridLayout_20->addWidget(label_51, 9, 4, 1, 1);

        speed_recipe = new QTextEdit(layoutWidget);
        speed_recipe->setObjectName(QString::fromUtf8("speed_recipe"));
        QSizePolicy sizePolicy10(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy10.setHorizontalStretch(0);
        sizePolicy10.setVerticalStretch(15);
        sizePolicy10.setHeightForWidth(speed_recipe->sizePolicy().hasHeightForWidth());
        speed_recipe->setSizePolicy(sizePolicy10);
        speed_recipe->setMinimumSize(QSize(0, 15));
        speed_recipe->setStyleSheet(QString::fromUtf8("font: 18px;\n"
"color: rgb(52, 101, 164);"));
        speed_recipe->setReadOnly(true);

        gridLayout_20->addWidget(speed_recipe, 1, 5, 1, 1);

        Y_relative_PH = new QTextEdit(layoutWidget);
        Y_relative_PH->setObjectName(QString::fromUtf8("Y_relative_PH"));
        Y_relative_PH->setEnabled(true);
        sizePolicy6.setHeightForWidth(Y_relative_PH->sizePolicy().hasHeightForWidth());
        Y_relative_PH->setSizePolicy(sizePolicy6);
        Y_relative_PH->setMinimumSize(QSize(15, 50));
        Y_relative_PH->setStyleSheet(QString::fromUtf8("font: 18px;\n"
"color: rgb(52, 101, 164);"));
        Y_relative_PH->setReadOnly(true);

        gridLayout_20->addWidget(Y_relative_PH, 10, 5, 1, 1);

        thickness_recipe = new QTextEdit(layoutWidget);
        thickness_recipe->setObjectName(QString::fromUtf8("thickness_recipe"));
        sizePolicy8.setHeightForWidth(thickness_recipe->sizePolicy().hasHeightForWidth());
        thickness_recipe->setSizePolicy(sizePolicy8);
        thickness_recipe->setMinimumSize(QSize(15, 15));
        thickness_recipe->setStyleSheet(QString::fromUtf8("font: 18px;\n"
"color: rgb(52, 101, 164);"));
        thickness_recipe->setReadOnly(true);

        gridLayout_20->addWidget(thickness_recipe, 1, 1, 1, 3);

        load_autoscan = new QPushButton(layoutWidget);
        load_autoscan->setObjectName(QString::fromUtf8("load_autoscan"));
        QSizePolicy sizePolicy11(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy11.setHorizontalStretch(0);
        sizePolicy11.setVerticalStretch(0);
        sizePolicy11.setHeightForWidth(load_autoscan->sizePolicy().hasHeightForWidth());
        load_autoscan->setSizePolicy(sizePolicy11);
        load_autoscan->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 8pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_20->addWidget(load_autoscan, 3, 4, 1, 1);

        x2_set = new QPushButton(layoutWidget);
        x2_set->setObjectName(QString::fromUtf8("x2_set"));
        sizePolicy2.setHeightForWidth(x2_set->sizePolicy().hasHeightForWidth());
        x2_set->setSizePolicy(sizePolicy2);
        x2_set->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_20->addWidget(x2_set, 10, 0, 1, 1);

        label_102 = new QLabel(layoutWidget);
        label_102->setObjectName(QString::fromUtf8("label_102"));
        sizePolicy5.setHeightForWidth(label_102->sizePolicy().hasHeightForWidth());
        label_102->setSizePolicy(sizePolicy5);
        label_102->setStyleSheet(QString::fromUtf8("\n"
"font: 700 16pt \"Ubuntu Mono\";"));

        gridLayout_20->addWidget(label_102, 5, 0, 4, 4);

        label_50 = new QLabel(layoutWidget);
        label_50->setObjectName(QString::fromUtf8("label_50"));
        QSizePolicy sizePolicy12(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy12.setHorizontalStretch(15);
        sizePolicy12.setVerticalStretch(0);
        sizePolicy12.setHeightForWidth(label_50->sizePolicy().hasHeightForWidth());
        label_50->setSizePolicy(sizePolicy12);
        label_50->setMinimumSize(QSize(15, 0));
        label_50->setStyleSheet(QString::fromUtf8("\n"
"font: 700 16pt \"Ubuntu Mono\";"));

        gridLayout_20->addWidget(label_50, 4, 4, 5, 2);

        label_95 = new QLabel(layoutWidget);
        label_95->setObjectName(QString::fromUtf8("label_95"));
        label_95->setStyleSheet(QString::fromUtf8("\n"
"font: 700 16pt \"Ubuntu Mono\";"));

        gridLayout_20->addWidget(label_95, 0, 4, 1, 2);

        x2_recipe = new QTextEdit(layoutWidget);
        x2_recipe->setObjectName(QString::fromUtf8("x2_recipe"));
        sizePolicy9.setHeightForWidth(x2_recipe->sizePolicy().hasHeightForWidth());
        x2_recipe->setSizePolicy(sizePolicy9);
        x2_recipe->setMinimumSize(QSize(15, 15));
        x2_recipe->setStyleSheet(QString::fromUtf8("font: 18px;\n"
"color: rgb(52, 101, 164);"));
        x2_recipe->setReadOnly(true);

        gridLayout_20->addWidget(x2_recipe, 10, 1, 1, 3);

        autoscan_recipe = new QPushButton(layoutWidget);
        autoscan_recipe->setObjectName(QString::fromUtf8("autoscan_recipe"));
        QSizePolicy sizePolicy13(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy13.setHorizontalStretch(0);
        sizePolicy13.setVerticalStretch(0);
        sizePolicy13.setHeightForWidth(autoscan_recipe->sizePolicy().hasHeightForWidth());
        autoscan_recipe->setSizePolicy(sizePolicy13);
        autoscan_recipe->setMinimumSize(QSize(0, 0));
        autoscan_recipe->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"font: 16pt \"Ubuntu Bold\";\n"
"	color: rgb(220, 0, 20);\n"
"}"));
        autoscan_recipe->setIconSize(QSize(32, 32));

        gridLayout_20->addWidget(autoscan_recipe, 3, 5, 1, 1);

        y2_recipe = new QTextEdit(layoutWidget);
        y2_recipe->setObjectName(QString::fromUtf8("y2_recipe"));
        sizePolicy9.setHeightForWidth(y2_recipe->sizePolicy().hasHeightForWidth());
        y2_recipe->setSizePolicy(sizePolicy9);
        y2_recipe->setMinimumSize(QSize(15, 15));
        y2_recipe->setStyleSheet(QString::fromUtf8("font: 18px;\n"
"color: rgb(52, 101, 164);"));
        y2_recipe->setReadOnly(true);

        gridLayout_20->addWidget(y2_recipe, 12, 1, 1, 3);


        recipeLayout->addLayout(gridLayout_20, 6, 0, 1, 3);

        save_recipe_button = new QPushButton(layoutWidget);
        save_recipe_button->setObjectName(QString::fromUtf8("save_recipe_button"));
        save_recipe_button->setMinimumSize(QSize(0, 50));
        save_recipe_button->setStyleSheet(QString::fromUtf8("\n"
"QPushButton {\n"
"color: white;\n"
"background-color: green;\n"
"font: 14pt \"Ubuntu Bold\";\n"
"}"));
        QIcon icon4;
        iconThemeName = QString::fromUtf8("media-floppy");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon4 = QIcon::fromTheme(iconThemeName);
        } else {
            icon4.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        save_recipe_button->setIcon(icon4);
        save_recipe_button->setIconSize(QSize(32, 32));

        recipeLayout->addWidget(save_recipe_button, 7, 0, 1, 3);

        label_31 = new QLabel(layoutWidget);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setStyleSheet(QString::fromUtf8("font: 16px;"));

        recipeLayout->addWidget(label_31, 0, 0, 1, 3);

        load_recipe_button = new QPushButton(layoutWidget);
        load_recipe_button->setObjectName(QString::fromUtf8("load_recipe_button"));
        load_recipe_button->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"color: white;\n"
"background-color: rgb(119, 136, 153);\n"
"font: 14pt \"Ubuntu Bold\";\n"
"}\n"
""));
        QIcon icon5;
        iconThemeName = QString::fromUtf8("document-open");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon5 = QIcon::fromTheme(iconThemeName);
        } else {
            icon5.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        load_recipe_button->setIcon(icon5);
        load_recipe_button->setIconSize(QSize(32, 32));

        recipeLayout->addWidget(load_recipe_button, 2, 0, 1, 3);

        name_recipe = new QTextEdit(layoutWidget);
        name_recipe->setObjectName(QString::fromUtf8("name_recipe"));
        QSizePolicy sizePolicy14(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy14.setHorizontalStretch(0);
        sizePolicy14.setVerticalStretch(0);
        sizePolicy14.setHeightForWidth(name_recipe->sizePolicy().hasHeightForWidth());
        name_recipe->setSizePolicy(sizePolicy14);
        name_recipe->setMaximumSize(QSize(16777215, 35));
        name_recipe->setStyleSheet(QString::fromUtf8("font: 16px;"));
        name_recipe->setReadOnly(true);

        recipeLayout->addWidget(name_recipe, 1, 0, 1, 3);

        layoutWidget1 = new QWidget(Dashboard);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(530, 30, 384, 771));
        Power_Layout_2 = new QHBoxLayout(layoutWidget1);
        Power_Layout_2->setSpacing(6);
        Power_Layout_2->setObjectName(QString::fromUtf8("Power_Layout_2"));
        Power_Layout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_18 = new QVBoxLayout();
        verticalLayout_18->setObjectName(QString::fromUtf8("verticalLayout_18"));
        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setObjectName(QString::fromUtf8("horizontalLayout_18"));
        RF_Actual_LCD = new QLCDNumber(layoutWidget1);
        RF_Actual_LCD->setObjectName(QString::fromUtf8("RF_Actual_LCD"));
        QSizePolicy sizePolicy15(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy15.setHorizontalStretch(0);
        sizePolicy15.setVerticalStretch(0);
        sizePolicy15.setHeightForWidth(RF_Actual_LCD->sizePolicy().hasHeightForWidth());
        RF_Actual_LCD->setSizePolicy(sizePolicy15);
        RF_Actual_LCD->setMinimumSize(QSize(125, 75));
        RF_Actual_LCD->setStyleSheet(QString::fromUtf8("color: rgb(204, 0, 0);"));
        RF_Actual_LCD->setFrameShape(QFrame::Box);
        RF_Actual_LCD->setFrameShadow(QFrame::Raised);
        RF_Actual_LCD->setDigitCount(3);
        RF_Actual_LCD->setProperty("value", QVariant(0.000000000000000));
        RF_Actual_LCD->setProperty("intValue", QVariant(0));

        horizontalLayout_18->addWidget(RF_Actual_LCD);

        RFfor_label_2 = new QLabel(layoutWidget1);
        RFfor_label_2->setObjectName(QString::fromUtf8("RFfor_label_2"));
        RFfor_label_2->setStyleSheet(QString::fromUtf8("font: 14pt \"Ubuntu\";"));
        RFfor_label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_18->addWidget(RFfor_label_2);


        verticalLayout_18->addLayout(horizontalLayout_18);

        RF_bar = new QProgressBar(layoutWidget1);
        RF_bar->setObjectName(QString::fromUtf8("RF_bar"));
        QSizePolicy sizePolicy16(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy16.setHorizontalStretch(0);
        sizePolicy16.setVerticalStretch(0);
        sizePolicy16.setHeightForWidth(RF_bar->sizePolicy().hasHeightForWidth());
        RF_bar->setSizePolicy(sizePolicy16);
        RF_bar->setMinimumSize(QSize(0, 55));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Century Gothic")});
        font1.setStyleStrategy(QFont::PreferDefault);
        RF_bar->setFont(font1);
        RF_bar->setMaximum(200);
        RF_bar->setValue(0);
        RF_bar->setTextVisible(false);
        RF_bar->setOrientation(Qt::Horizontal);

        verticalLayout_18->addWidget(RF_bar);

        RefRF_bar = new QProgressBar(layoutWidget1);
        RefRF_bar->setObjectName(QString::fromUtf8("RefRF_bar"));
        QSizePolicy sizePolicy17(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy17.setHorizontalStretch(0);
        sizePolicy17.setVerticalStretch(0);
        sizePolicy17.setHeightForWidth(RefRF_bar->sizePolicy().hasHeightForWidth());
        RefRF_bar->setSizePolicy(sizePolicy17);
        RefRF_bar->setMinimumSize(QSize(0, 55));
        RefRF_bar->setStyleSheet(QString::fromUtf8("QProgressBar \n"
"{		\n"
"	border-radius: 26px;\n"
"	background-color: #d3dbdd;\n"
"}\n"
"\n"
"QProgressBar::Chunk\n"
"{\n"
"    border-radius: 26px;\n"
"	background-color:  rgb(255, 242, 0);\n"
"}\n"
""));
        RefRF_bar->setMaximum(30);
        RefRF_bar->setValue(0);
        RefRF_bar->setTextVisible(false);
        RefRF_bar->setOrientation(Qt::Horizontal);
        RefRF_bar->setInvertedAppearance(true);

        verticalLayout_18->addWidget(RefRF_bar);

        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setObjectName(QString::fromUtf8("horizontalLayout_20"));
        ref_label_3 = new QLabel(layoutWidget1);
        ref_label_3->setObjectName(QString::fromUtf8("ref_label_3"));
        ref_label_3->setStyleSheet(QString::fromUtf8("font: 14pt \"Ubuntu\";"));
        ref_label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_20->addWidget(ref_label_3);

        RefRF_Actual_LCD = new QLCDNumber(layoutWidget1);
        RefRF_Actual_LCD->setObjectName(QString::fromUtf8("RefRF_Actual_LCD"));
        sizePolicy15.setHeightForWidth(RefRF_Actual_LCD->sizePolicy().hasHeightForWidth());
        RefRF_Actual_LCD->setSizePolicy(sizePolicy15);
        RefRF_Actual_LCD->setMinimumSize(QSize(125, 75));
        RefRF_Actual_LCD->setStyleSheet(QString::fromUtf8("color:  rgb(255, 242, 0);\n"
""));
        RefRF_Actual_LCD->setFrameShape(QFrame::Box);
        RefRF_Actual_LCD->setFrameShadow(QFrame::Raised);
        RefRF_Actual_LCD->setDigitCount(3);
        RefRF_Actual_LCD->setProperty("intValue", QVariant(0));

        horizontalLayout_20->addWidget(RefRF_Actual_LCD);


        verticalLayout_18->addLayout(horizontalLayout_20);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setObjectName(QString::fromUtf8("horizontalLayout_19"));
        MB_Actual_LCD = new QLCDNumber(layoutWidget1);
        MB_Actual_LCD->setObjectName(QString::fromUtf8("MB_Actual_LCD"));
        sizePolicy15.setHeightForWidth(MB_Actual_LCD->sizePolicy().hasHeightForWidth());
        MB_Actual_LCD->setSizePolicy(sizePolicy15);
        MB_Actual_LCD->setMinimumSize(QSize(125, 75));
        MB_Actual_LCD->setStyleSheet(QString::fromUtf8("color: black;"));
        MB_Actual_LCD->setFrameShape(QFrame::Box);
        MB_Actual_LCD->setFrameShadow(QFrame::Raised);
        MB_Actual_LCD->setDigitCount(5);
        MB_Actual_LCD->setProperty("intValue", QVariant(0));

        horizontalLayout_19->addWidget(MB_Actual_LCD);

        MBfor_label_3 = new QLabel(layoutWidget1);
        MBfor_label_3->setObjectName(QString::fromUtf8("MBfor_label_3"));
        MBfor_label_3->setStyleSheet(QString::fromUtf8("font: 14pt \"Ubuntu\";"));
        MBfor_label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_19->addWidget(MBfor_label_3);


        verticalLayout_18->addLayout(horizontalLayout_19);

        MB_Pos_Bar = new QProgressBar(layoutWidget1);
        MB_Pos_Bar->setObjectName(QString::fromUtf8("MB_Pos_Bar"));
        sizePolicy17.setHeightForWidth(MB_Pos_Bar->sizePolicy().hasHeightForWidth());
        MB_Pos_Bar->setSizePolicy(sizePolicy17);
        MB_Pos_Bar->setMinimumSize(QSize(0, 55));
        MB_Pos_Bar->setStyleSheet(QString::fromUtf8("QProgressBar::Chunk {\n"
"background-color: black;\n"
"}"));
        MB_Pos_Bar->setValue(0);
        MB_Pos_Bar->setTextVisible(false);
        MB_Pos_Bar->setOrientation(Qt::Horizontal);

        verticalLayout_18->addWidget(MB_Pos_Bar);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        MB_Left_Button = new QPushButton(layoutWidget1);
        MB_Left_Button->setObjectName(QString::fromUtf8("MB_Left_Button"));
        QSizePolicy sizePolicy18(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
        sizePolicy18.setHorizontalStretch(0);
        sizePolicy18.setVerticalStretch(0);
        sizePolicy18.setHeightForWidth(MB_Left_Button->sizePolicy().hasHeightForWidth());
        MB_Left_Button->setSizePolicy(sizePolicy18);
        MB_Left_Button->setLayoutDirection(Qt::LeftToRight);
        MB_Left_Button->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));
        MB_Left_Button->setIconSize(QSize(128, 64));
        MB_Left_Button->setCheckable(false);

        horizontalLayout_13->addWidget(MB_Left_Button);

        verticalLayout_15 = new QVBoxLayout();
        verticalLayout_15->setObjectName(QString::fromUtf8("verticalLayout_15"));
        stepLabel = new QLabel(layoutWidget1);
        stepLabel->setObjectName(QString::fromUtf8("stepLabel"));
        sizePolicy17.setHeightForWidth(stepLabel->sizePolicy().hasHeightForWidth());
        stepLabel->setSizePolicy(sizePolicy17);

        verticalLayout_15->addWidget(stepLabel);

        stepSizeBox = new QComboBox(layoutWidget1);
        stepSizeBox->addItem(QString());
        stepSizeBox->addItem(QString());
        stepSizeBox->addItem(QString());
        stepSizeBox->setObjectName(QString::fromUtf8("stepSizeBox"));

        verticalLayout_15->addWidget(stepSizeBox);


        horizontalLayout_13->addLayout(verticalLayout_15);

        MB_Right_Button = new QPushButton(layoutWidget1);
        MB_Right_Button->setObjectName(QString::fromUtf8("MB_Right_Button"));
        sizePolicy18.setHeightForWidth(MB_Right_Button->sizePolicy().hasHeightForWidth());
        MB_Right_Button->setSizePolicy(sizePolicy18);
        MB_Right_Button->setLayoutDirection(Qt::LeftToRight);
        MB_Right_Button->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));
        MB_Right_Button->setIconSize(QSize(128, 64));
        MB_Right_Button->setCheckable(false);

        horizontalLayout_13->addWidget(MB_Right_Button);


        verticalLayout_18->addLayout(horizontalLayout_13);

        horizontalLayout_22 = new QHBoxLayout();
        horizontalLayout_22->setObjectName(QString::fromUtf8("horizontalLayout_22"));
        ref_label_4 = new QLabel(layoutWidget1);
        ref_label_4->setObjectName(QString::fromUtf8("ref_label_4"));
        ref_label_4->setStyleSheet(QString::fromUtf8("font: 14pt \"Ubuntu\";"));
        ref_label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_22->addWidget(ref_label_4);

        temp_LCD = new QLCDNumber(layoutWidget1);
        temp_LCD->setObjectName(QString::fromUtf8("temp_LCD"));
        sizePolicy15.setHeightForWidth(temp_LCD->sizePolicy().hasHeightForWidth());
        temp_LCD->setSizePolicy(sizePolicy15);
        temp_LCD->setMinimumSize(QSize(125, 75));
        temp_LCD->setStyleSheet(QString::fromUtf8("color: blue;"));
        temp_LCD->setFrameShape(QFrame::Box);
        temp_LCD->setFrameShadow(QFrame::Raised);
        temp_LCD->setDigitCount(3);
        temp_LCD->setProperty("intValue", QVariant(0));

        horizontalLayout_22->addWidget(temp_LCD);


        verticalLayout_18->addLayout(horizontalLayout_22);

        Temp_bar = new QProgressBar(layoutWidget1);
        Temp_bar->setObjectName(QString::fromUtf8("Temp_bar"));
        sizePolicy17.setHeightForWidth(Temp_bar->sizePolicy().hasHeightForWidth());
        Temp_bar->setSizePolicy(sizePolicy17);
        Temp_bar->setMinimumSize(QSize(0, 55));
        Temp_bar->setStyleSheet(QString::fromUtf8("QProgressBar::Chunk {\n"
"background-color: blue;\n"
"}"));
        Temp_bar->setMaximum(150);
        Temp_bar->setValue(0);
        Temp_bar->setTextVisible(false);
        Temp_bar->setOrientation(Qt::Horizontal);

        verticalLayout_18->addWidget(Temp_bar);


        Power_Layout_2->addLayout(verticalLayout_18);

        verticalLayoutWidget = new QWidget(Dashboard);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(50, 30, 451, 771));
        gasses = new QVBoxLayout(verticalLayoutWidget);
        gasses->setObjectName(QString::fromUtf8("gasses"));
        gasses->setContentsMargins(0, 0, 0, 0);
        Gasses_Layout_2 = new QHBoxLayout();
        Gasses_Layout_2->setSpacing(10);
        Gasses_Layout_2->setObjectName(QString::fromUtf8("Gasses_Layout_2"));
        Gas1_Layout_2 = new QVBoxLayout();
        Gas1_Layout_2->setObjectName(QString::fromUtf8("Gas1_Layout_2"));
        Gas1_Layout_2->setContentsMargins(-1, -1, -1, 0);
        gas1_label = new QLabel(verticalLayoutWidget);
        gas1_label->setObjectName(QString::fromUtf8("gas1_label"));
        gas1_label->setAlignment(Qt::AlignCenter);

        Gas1_Layout_2->addWidget(gas1_label);

        gas1_progress_slider_Layout_2 = new QHBoxLayout();
        gas1_progress_slider_Layout_2->setObjectName(QString::fromUtf8("gas1_progress_slider_Layout_2"));
        gas1_sliderbar = new QSlider(verticalLayoutWidget);
        gas1_sliderbar->setObjectName(QString::fromUtf8("gas1_sliderbar"));
        gas1_sliderbar->setEnabled(false);
        gas1_sliderbar->setMaximum(2000);
        gas1_sliderbar->setValue(0);
        gas1_sliderbar->setOrientation(Qt::Vertical);

        gas1_progress_slider_Layout_2->addWidget(gas1_sliderbar);

        gas1_progressbar = new QProgressBar(verticalLayoutWidget);
        gas1_progressbar->setObjectName(QString::fromUtf8("gas1_progressbar"));
        QSizePolicy sizePolicy19(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy19.setHorizontalStretch(0);
        sizePolicy19.setVerticalStretch(0);
        sizePolicy19.setHeightForWidth(gas1_progressbar->sizePolicy().hasHeightForWidth());
        gas1_progressbar->setSizePolicy(sizePolicy19);
        gas1_progressbar->setMinimumSize(QSize(55, 0));
        gas1_progressbar->setMaximum(20);
        gas1_progressbar->setValue(0);
        gas1_progressbar->setTextVisible(false);
        gas1_progressbar->setOrientation(Qt::Vertical);

        gas1_progress_slider_Layout_2->addWidget(gas1_progressbar);


        Gas1_Layout_2->addLayout(gas1_progress_slider_Layout_2);

        gas1_button_readout_layout_2 = new QHBoxLayout();
        gas1_button_readout_layout_2->setSpacing(0);
        gas1_button_readout_layout_2->setObjectName(QString::fromUtf8("gas1_button_readout_layout_2"));
        gas1_button_readout_layout_2->setContentsMargins(-1, -1, 0, 0);
        gas1_recipe_actual_SLPM_2 = new QVBoxLayout();
        gas1_recipe_actual_SLPM_2->setSpacing(0);
        gas1_recipe_actual_SLPM_2->setObjectName(QString::fromUtf8("gas1_recipe_actual_SLPM_2"));
        gas1_recipe_actual_SLPM_2->setContentsMargins(-1, -1, 0, -1);
        gas1_actual_SLPM = new QLabel(verticalLayoutWidget);
        gas1_actual_SLPM->setObjectName(QString::fromUtf8("gas1_actual_SLPM"));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Arial Bold")});
        font2.setPointSize(16);
        font2.setBold(false);
        font2.setItalic(false);
        gas1_actual_SLPM->setFont(font2);
        gas1_actual_SLPM->setStyleSheet(QString::fromUtf8("QLabel {\n"
"color: red;\n"
"font: 16pt \"Arial Bold\";\n"
"}"));
        gas1_actual_SLPM->setFrameShape(QFrame::NoFrame);
        gas1_actual_SLPM->setFrameShadow(QFrame::Plain);
        gas1_actual_SLPM->setTextFormat(Qt::AutoText);
        gas1_actual_SLPM->setScaledContents(true);
        gas1_actual_SLPM->setAlignment(Qt::AlignCenter);
        gas1_actual_SLPM->setTextInteractionFlags(Qt::NoTextInteraction);

        gas1_recipe_actual_SLPM_2->addWidget(gas1_actual_SLPM);

        gas1_recipe_SLPM = new QLabel(verticalLayoutWidget);
        gas1_recipe_SLPM->setObjectName(QString::fromUtf8("gas1_recipe_SLPM"));
        QSizePolicy sizePolicy20(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy20.setHorizontalStretch(0);
        sizePolicy20.setVerticalStretch(0);
        sizePolicy20.setHeightForWidth(gas1_recipe_SLPM->sizePolicy().hasHeightForWidth());
        gas1_recipe_SLPM->setSizePolicy(sizePolicy20);
        gas1_recipe_SLPM->setMinimumSize(QSize(75, 0));
        gas1_recipe_SLPM->setFont(font2);
        gas1_recipe_SLPM->setStyleSheet(QString::fromUtf8("QLabel {\n"
"color: #bfd5ed;\n"
"font: 16pt \"Arial Bold\";\n"
"}"));
        gas1_recipe_SLPM->setFrameShape(QFrame::NoFrame);
        gas1_recipe_SLPM->setFrameShadow(QFrame::Plain);
        gas1_recipe_SLPM->setTextFormat(Qt::AutoText);
        gas1_recipe_SLPM->setScaledContents(true);
        gas1_recipe_SLPM->setAlignment(Qt::AlignCenter);
        gas1_recipe_SLPM->setTextInteractionFlags(Qt::NoTextInteraction);

        gas1_recipe_actual_SLPM_2->addWidget(gas1_recipe_SLPM);


        gas1_button_readout_layout_2->addLayout(gas1_recipe_actual_SLPM_2);


        Gas1_Layout_2->addLayout(gas1_button_readout_layout_2);


        Gasses_Layout_2->addLayout(Gas1_Layout_2);

        Gas2_Layout_2 = new QVBoxLayout();
        Gas2_Layout_2->setObjectName(QString::fromUtf8("Gas2_Layout_2"));
        Gas2_Layout_2->setContentsMargins(-1, -1, -1, 0);
        gas2_label = new QLabel(verticalLayoutWidget);
        gas2_label->setObjectName(QString::fromUtf8("gas2_label"));
        gas2_label->setAlignment(Qt::AlignCenter);

        Gas2_Layout_2->addWidget(gas2_label);

        gas2_progress_slider_layout_2 = new QHBoxLayout();
        gas2_progress_slider_layout_2->setObjectName(QString::fromUtf8("gas2_progress_slider_layout_2"));
        gas2_sliderbar = new QSlider(verticalLayoutWidget);
        gas2_sliderbar->setObjectName(QString::fromUtf8("gas2_sliderbar"));
        gas2_sliderbar->setEnabled(false);
        gas2_sliderbar->setMaximum(2000);
        gas2_sliderbar->setOrientation(Qt::Vertical);

        gas2_progress_slider_layout_2->addWidget(gas2_sliderbar);

        gas2_progressbar = new QProgressBar(verticalLayoutWidget);
        gas2_progressbar->setObjectName(QString::fromUtf8("gas2_progressbar"));
        sizePolicy19.setHeightForWidth(gas2_progressbar->sizePolicy().hasHeightForWidth());
        gas2_progressbar->setSizePolicy(sizePolicy19);
        gas2_progressbar->setMinimumSize(QSize(55, 0));
        gas2_progressbar->setMaximum(20);
        gas2_progressbar->setValue(0);
        gas2_progressbar->setTextVisible(false);
        gas2_progressbar->setOrientation(Qt::Vertical);

        gas2_progress_slider_layout_2->addWidget(gas2_progressbar);


        Gas2_Layout_2->addLayout(gas2_progress_slider_layout_2);

        gas2_buttton_readout_layout_2 = new QHBoxLayout();
        gas2_buttton_readout_layout_2->setObjectName(QString::fromUtf8("gas2_buttton_readout_layout_2"));
        gas2_recipe_actual_SLPM_2 = new QVBoxLayout();
        gas2_recipe_actual_SLPM_2->setSpacing(0);
        gas2_recipe_actual_SLPM_2->setObjectName(QString::fromUtf8("gas2_recipe_actual_SLPM_2"));
        gas2_actual_SLPM = new QLabel(verticalLayoutWidget);
        gas2_actual_SLPM->setObjectName(QString::fromUtf8("gas2_actual_SLPM"));
        gas2_actual_SLPM->setFont(font2);
        gas2_actual_SLPM->setStyleSheet(QString::fromUtf8("QLabel {\n"
"color: red;\n"
"font: 16pt \"Arial Bold\";\n"
"}"));
        gas2_actual_SLPM->setFrameShape(QFrame::NoFrame);
        gas2_actual_SLPM->setFrameShadow(QFrame::Plain);
        gas2_actual_SLPM->setTextFormat(Qt::AutoText);
        gas2_actual_SLPM->setScaledContents(true);
        gas2_actual_SLPM->setAlignment(Qt::AlignCenter);
        gas2_actual_SLPM->setTextInteractionFlags(Qt::NoTextInteraction);

        gas2_recipe_actual_SLPM_2->addWidget(gas2_actual_SLPM);

        gas2_recipe_SLPM = new QLabel(verticalLayoutWidget);
        gas2_recipe_SLPM->setObjectName(QString::fromUtf8("gas2_recipe_SLPM"));
        sizePolicy20.setHeightForWidth(gas2_recipe_SLPM->sizePolicy().hasHeightForWidth());
        gas2_recipe_SLPM->setSizePolicy(sizePolicy20);
        gas2_recipe_SLPM->setMinimumSize(QSize(75, 0));
        gas2_recipe_SLPM->setFont(font2);
        gas2_recipe_SLPM->setStyleSheet(QString::fromUtf8("QLabel {\n"
"color: #bfd5ed;\n"
"font: 16pt \"Arial Bold\";\n"
"}"));
        gas2_recipe_SLPM->setFrameShape(QFrame::NoFrame);
        gas2_recipe_SLPM->setFrameShadow(QFrame::Plain);
        gas2_recipe_SLPM->setTextFormat(Qt::AutoText);
        gas2_recipe_SLPM->setScaledContents(true);
        gas2_recipe_SLPM->setAlignment(Qt::AlignCenter);
        gas2_recipe_SLPM->setTextInteractionFlags(Qt::NoTextInteraction);

        gas2_recipe_actual_SLPM_2->addWidget(gas2_recipe_SLPM);


        gas2_buttton_readout_layout_2->addLayout(gas2_recipe_actual_SLPM_2);


        Gas2_Layout_2->addLayout(gas2_buttton_readout_layout_2);


        Gasses_Layout_2->addLayout(Gas2_Layout_2);

        Gas3_Layout_2 = new QVBoxLayout();
        Gas3_Layout_2->setObjectName(QString::fromUtf8("Gas3_Layout_2"));
        Gas3_Layout_2->setContentsMargins(-1, -1, -1, 0);
        gas3_label = new QLabel(verticalLayoutWidget);
        gas3_label->setObjectName(QString::fromUtf8("gas3_label"));
        gas3_label->setAlignment(Qt::AlignCenter);

        Gas3_Layout_2->addWidget(gas3_label);

        horizontalLayout_25 = new QHBoxLayout();
        horizontalLayout_25->setObjectName(QString::fromUtf8("horizontalLayout_25"));
        gas3_sliderbar = new QSlider(verticalLayoutWidget);
        gas3_sliderbar->setObjectName(QString::fromUtf8("gas3_sliderbar"));
        gas3_sliderbar->setEnabled(false);
        gas3_sliderbar->setMaximum(30);
        gas3_sliderbar->setOrientation(Qt::Vertical);

        horizontalLayout_25->addWidget(gas3_sliderbar);

        gas3_progessbar = new QProgressBar(verticalLayoutWidget);
        gas3_progessbar->setObjectName(QString::fromUtf8("gas3_progessbar"));
        sizePolicy19.setHeightForWidth(gas3_progessbar->sizePolicy().hasHeightForWidth());
        gas3_progessbar->setSizePolicy(sizePolicy19);
        gas3_progessbar->setMinimumSize(QSize(55, 0));
        gas3_progessbar->setMaximum(30);
        gas3_progessbar->setValue(0);
        gas3_progessbar->setTextVisible(false);
        gas3_progessbar->setOrientation(Qt::Vertical);

        horizontalLayout_25->addWidget(gas3_progessbar);


        Gas3_Layout_2->addLayout(horizontalLayout_25);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        verticalLayout_16 = new QVBoxLayout();
        verticalLayout_16->setSpacing(0);
        verticalLayout_16->setObjectName(QString::fromUtf8("verticalLayout_16"));
        gas3_actual_SLPM = new QLabel(verticalLayoutWidget);
        gas3_actual_SLPM->setObjectName(QString::fromUtf8("gas3_actual_SLPM"));
        gas3_actual_SLPM->setFont(font2);
        gas3_actual_SLPM->setStyleSheet(QString::fromUtf8("QLabel {\n"
"color: red;\n"
"font: 16pt \"Arial Bold\";\n"
"}"));
        gas3_actual_SLPM->setFrameShape(QFrame::NoFrame);
        gas3_actual_SLPM->setFrameShadow(QFrame::Plain);
        gas3_actual_SLPM->setTextFormat(Qt::AutoText);
        gas3_actual_SLPM->setScaledContents(true);
        gas3_actual_SLPM->setAlignment(Qt::AlignCenter);
        gas3_actual_SLPM->setIndent(1);
        gas3_actual_SLPM->setTextInteractionFlags(Qt::NoTextInteraction);

        verticalLayout_16->addWidget(gas3_actual_SLPM);

        gas3_recipe_SLPM = new QLabel(verticalLayoutWidget);
        gas3_recipe_SLPM->setObjectName(QString::fromUtf8("gas3_recipe_SLPM"));
        sizePolicy20.setHeightForWidth(gas3_recipe_SLPM->sizePolicy().hasHeightForWidth());
        gas3_recipe_SLPM->setSizePolicy(sizePolicy20);
        gas3_recipe_SLPM->setMinimumSize(QSize(75, 0));
        gas3_recipe_SLPM->setFont(font2);
        gas3_recipe_SLPM->setStyleSheet(QString::fromUtf8("QLabel {\n"
"color: #bfd5ed;\n"
"font: 16pt \"Arial Bold\";\n"
"}"));
        gas3_recipe_SLPM->setFrameShape(QFrame::NoFrame);
        gas3_recipe_SLPM->setFrameShadow(QFrame::Plain);
        gas3_recipe_SLPM->setTextFormat(Qt::AutoText);
        gas3_recipe_SLPM->setScaledContents(true);
        gas3_recipe_SLPM->setAlignment(Qt::AlignCenter);
        gas3_recipe_SLPM->setIndent(1);
        gas3_recipe_SLPM->setTextInteractionFlags(Qt::NoTextInteraction);

        verticalLayout_16->addWidget(gas3_recipe_SLPM);


        horizontalLayout_16->addLayout(verticalLayout_16);


        Gas3_Layout_2->addLayout(horizontalLayout_16);


        Gasses_Layout_2->addLayout(Gas3_Layout_2);

        Gas4_Layout_2 = new QVBoxLayout();
        Gas4_Layout_2->setObjectName(QString::fromUtf8("Gas4_Layout_2"));
        Gas4_Layout_2->setContentsMargins(-1, -1, -1, 0);
        gas4_label = new QLabel(verticalLayoutWidget);
        gas4_label->setObjectName(QString::fromUtf8("gas4_label"));
        gas4_label->setAlignment(Qt::AlignCenter);

        Gas4_Layout_2->addWidget(gas4_label);

        horizontalLayout_27 = new QHBoxLayout();
        horizontalLayout_27->setObjectName(QString::fromUtf8("horizontalLayout_27"));
        gas4_sliderbar = new QSlider(verticalLayoutWidget);
        gas4_sliderbar->setObjectName(QString::fromUtf8("gas4_sliderbar"));
        gas4_sliderbar->setEnabled(false);
        gas4_sliderbar->setMinimum(0);
        gas4_sliderbar->setMaximum(30);
        gas4_sliderbar->setOrientation(Qt::Vertical);
        gas4_sliderbar->setTickInterval(1);

        horizontalLayout_27->addWidget(gas4_sliderbar);

        gas4_progressbar = new QProgressBar(verticalLayoutWidget);
        gas4_progressbar->setObjectName(QString::fromUtf8("gas4_progressbar"));
        sizePolicy19.setHeightForWidth(gas4_progressbar->sizePolicy().hasHeightForWidth());
        gas4_progressbar->setSizePolicy(sizePolicy19);
        gas4_progressbar->setMinimumSize(QSize(55, 0));
        gas4_progressbar->setMaximum(30);
        gas4_progressbar->setValue(0);
        gas4_progressbar->setTextVisible(false);
        gas4_progressbar->setOrientation(Qt::Vertical);

        horizontalLayout_27->addWidget(gas4_progressbar);


        Gas4_Layout_2->addLayout(horizontalLayout_27);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));
        verticalLayout_17 = new QVBoxLayout();
        verticalLayout_17->setSpacing(0);
        verticalLayout_17->setObjectName(QString::fromUtf8("verticalLayout_17"));
        gas4_actual_SLPM = new QLabel(verticalLayoutWidget);
        gas4_actual_SLPM->setObjectName(QString::fromUtf8("gas4_actual_SLPM"));
        gas4_actual_SLPM->setFont(font2);
        gas4_actual_SLPM->setStyleSheet(QString::fromUtf8("QLabel {\n"
"color: red;\n"
"font: 16pt \"Arial Bold\";\n"
"}"));
        gas4_actual_SLPM->setFrameShape(QFrame::NoFrame);
        gas4_actual_SLPM->setFrameShadow(QFrame::Plain);
        gas4_actual_SLPM->setTextFormat(Qt::AutoText);
        gas4_actual_SLPM->setScaledContents(true);
        gas4_actual_SLPM->setAlignment(Qt::AlignCenter);
        gas4_actual_SLPM->setTextInteractionFlags(Qt::NoTextInteraction);

        verticalLayout_17->addWidget(gas4_actual_SLPM);

        gas4_recipe_SLPM = new QLabel(verticalLayoutWidget);
        gas4_recipe_SLPM->setObjectName(QString::fromUtf8("gas4_recipe_SLPM"));
        sizePolicy20.setHeightForWidth(gas4_recipe_SLPM->sizePolicy().hasHeightForWidth());
        gas4_recipe_SLPM->setSizePolicy(sizePolicy20);
        gas4_recipe_SLPM->setMinimumSize(QSize(75, 0));
        gas4_recipe_SLPM->setFont(font2);
        gas4_recipe_SLPM->setStyleSheet(QString::fromUtf8("QLabel {\n"
"color: #bfd5ed;\n"
"font: 16pt \"Arial Bold\";\n"
"}"));
        gas4_recipe_SLPM->setFrameShape(QFrame::NoFrame);
        gas4_recipe_SLPM->setFrameShadow(QFrame::Plain);
        gas4_recipe_SLPM->setTextFormat(Qt::AutoText);
        gas4_recipe_SLPM->setScaledContents(true);
        gas4_recipe_SLPM->setAlignment(Qt::AlignCenter);
        gas4_recipe_SLPM->setTextInteractionFlags(Qt::NoTextInteraction);

        verticalLayout_17->addWidget(gas4_recipe_SLPM);


        horizontalLayout_17->addLayout(verticalLayout_17);


        Gas4_Layout_2->addLayout(horizontalLayout_17);


        Gasses_Layout_2->addLayout(Gas4_Layout_2);


        gasses->addLayout(Gasses_Layout_2);

        plsmaBtn = new QPushButton(verticalLayoutWidget);
        plsmaBtn->setObjectName(QString::fromUtf8("plsmaBtn"));
        QSizePolicy sizePolicy21(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy21.setHorizontalStretch(0);
        sizePolicy21.setVerticalStretch(60);
        sizePolicy21.setHeightForWidth(plsmaBtn->sizePolicy().hasHeightForWidth());
        plsmaBtn->setSizePolicy(sizePolicy21);
        plsmaBtn->setMinimumSize(QSize(150, 100));
        plsmaBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: blue;\n"
"font: 28pt \"Ubuntu Bold\";\n"
"}"));
        plsmaBtn->setCheckable(true);

        gasses->addWidget(plsmaBtn);

        layoutWidget2 = new QWidget(Dashboard);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(1370, 660, 221, 194));
        verticalLayout_21 = new QVBoxLayout(layoutWidget2);
        verticalLayout_21->setObjectName(QString::fromUtf8("verticalLayout_21"));
        verticalLayout_21->setContentsMargins(0, 0, 0, 0);
        label_52 = new QLabel(layoutWidget2);
        label_52->setObjectName(QString::fromUtf8("label_52"));

        verticalLayout_21->addWidget(label_52);

        axisstatus = new QTextEdit(layoutWidget2);
        axisstatus->setObjectName(QString::fromUtf8("axisstatus"));
        axisstatus->setEnabled(true);
        axisstatus->setReadOnly(true);

        verticalLayout_21->addWidget(axisstatus);

        label_53 = new QLabel(layoutWidget2);
        label_53->setObjectName(QString::fromUtf8("label_53"));

        verticalLayout_21->addWidget(label_53);

        axisstatus_2 = new QTextEdit(layoutWidget2);
        axisstatus_2->setObjectName(QString::fromUtf8("axisstatus_2"));
        axisstatus_2->setEnabled(true);
        axisstatus_2->setReadOnly(true);

        verticalLayout_21->addWidget(axisstatus_2);

        gridLayoutWidget_7 = new QWidget(Dashboard);
        gridLayoutWidget_7->setObjectName(QString::fromUtf8("gridLayoutWidget_7"));
        gridLayoutWidget_7->setGeometry(QRect(1370, 30, 224, 611));
        gridLayout_8 = new QGridLayout(gridLayoutWidget_7);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        gridLayout_8->setContentsMargins(0, 0, 0, 0);
        vac_button = new QPushButton(gridLayoutWidget_7);
        vac_button->setObjectName(QString::fromUtf8("vac_button"));
        QSizePolicy sizePolicy22(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
        sizePolicy22.setHorizontalStretch(0);
        sizePolicy22.setVerticalStretch(0);
        sizePolicy22.setHeightForWidth(vac_button->sizePolicy().hasHeightForWidth());
        vac_button->setSizePolicy(sizePolicy22);
        vac_button->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: gray;\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));
        vac_button->setCheckable(true);

        gridLayout_8->addWidget(vac_button, 3, 0, 1, 1);

        twospot_button = new QPushButton(gridLayoutWidget_7);
        twospot_button->setObjectName(QString::fromUtf8("twospot_button"));
        sizePolicy22.setHeightForWidth(twospot_button->sizePolicy().hasHeightForWidth());
        twospot_button->setSizePolicy(sizePolicy22);
        twospot_button->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 14pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_8->addWidget(twospot_button, 3, 1, 1, 1);

        init_button = new QPushButton(gridLayoutWidget_7);
        init_button->setObjectName(QString::fromUtf8("init_button"));
        sizePolicy22.setHeightForWidth(init_button->sizePolicy().hasHeightForWidth());
        init_button->setSizePolicy(sizePolicy22);
        init_button->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 14pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_8->addWidget(init_button, 2, 1, 1, 1);

        Joystick_button = new QPushButton(gridLayoutWidget_7);
        Joystick_button->setObjectName(QString::fromUtf8("Joystick_button"));
        sizePolicy22.setHeightForWidth(Joystick_button->sizePolicy().hasHeightForWidth());
        Joystick_button->setSizePolicy(sizePolicy22);
        Joystick_button->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: gray;\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));
        Joystick_button->setCheckable(true);

        gridLayout_8->addWidget(Joystick_button, 2, 0, 1, 1);

        verticalLayout_22 = new QVBoxLayout();
        verticalLayout_22->setObjectName(QString::fromUtf8("verticalLayout_22"));
        label_55 = new QLabel(gridLayoutWidget_7);
        label_55->setObjectName(QString::fromUtf8("label_55"));
        QSizePolicy sizePolicy23(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy23.setHorizontalStretch(0);
        sizePolicy23.setVerticalStretch(0);
        sizePolicy23.setHeightForWidth(label_55->sizePolicy().hasHeightForWidth());
        label_55->setSizePolicy(sizePolicy23);
        label_55->setMaximumSize(QSize(16777215, 30));
        label_55->setStyleSheet(QString::fromUtf8("QLabel {\n"
"font: 8pt \"Ubuntu Bold\";\n"
"}"));
        label_55->setAlignment(Qt::AlignCenter);

        verticalLayout_22->addWidget(label_55);

        wafer_diameter = new QComboBox(gridLayoutWidget_7);
        wafer_diameter->addItem(QString());
        wafer_diameter->addItem(QString());
        wafer_diameter->addItem(QString());
        wafer_diameter->addItem(QString());
        wafer_diameter->setObjectName(QString::fromUtf8("wafer_diameter"));
        sizePolicy23.setHeightForWidth(wafer_diameter->sizePolicy().hasHeightForWidth());
        wafer_diameter->setSizePolicy(sizePolicy23);
        wafer_diameter->setMaximumSize(QSize(200, 50));
        wafer_diameter->setLayoutDirection(Qt::RightToLeft);
        wafer_diameter->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"background-color: gray;\n"
"font: 16pt \"Ubuntu Bold\";\n"
"}"));

        verticalLayout_22->addWidget(wafer_diameter);


        gridLayout_8->addLayout(verticalLayout_22, 7, 0, 1, 1);

        label_54 = new QLabel(gridLayoutWidget_7);
        label_54->setObjectName(QString::fromUtf8("label_54"));

        gridLayout_8->addWidget(label_54, 0, 0, 1, 1);

        scan_button = new QPushButton(gridLayoutWidget_7);
        scan_button->setObjectName(QString::fromUtf8("scan_button"));
        sizePolicy22.setHeightForWidth(scan_button->sizePolicy().hasHeightForWidth());
        scan_button->setSizePolicy(sizePolicy22);
        scan_button->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_8->addWidget(scan_button, 4, 1, 1, 1);

        diameter_button = new QPushButton(gridLayoutWidget_7);
        diameter_button->setObjectName(QString::fromUtf8("diameter_button"));
        sizePolicy22.setHeightForWidth(diameter_button->sizePolicy().hasHeightForWidth());
        diameter_button->setSizePolicy(sizePolicy22);
        diameter_button->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 14pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_8->addWidget(diameter_button, 7, 1, 1, 1);

        Home_button = new QPushButton(gridLayoutWidget_7);
        Home_button->setObjectName(QString::fromUtf8("Home_button"));
        sizePolicy22.setHeightForWidth(Home_button->sizePolicy().hasHeightForWidth());
        Home_button->setSizePolicy(sizePolicy22);
        Home_button->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_8->addWidget(Home_button, 5, 1, 1, 1);

        Stagepins_button = new QPushButton(gridLayoutWidget_7);
        Stagepins_button->setObjectName(QString::fromUtf8("Stagepins_button"));
        sizePolicy22.setHeightForWidth(Stagepins_button->sizePolicy().hasHeightForWidth());
        Stagepins_button->setSizePolicy(sizePolicy22);
        Stagepins_button->setStyleSheet(QString::fromUtf8("QPushButton {background-color: gray;\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));
        Stagepins_button->setCheckable(true);
        Stagepins_button->setChecked(false);

        gridLayout_8->addWidget(Stagepins_button, 5, 0, 1, 1);

        n2_purge_button = new QPushButton(gridLayoutWidget_7);
        n2_purge_button->setObjectName(QString::fromUtf8("n2_purge_button"));
        sizePolicy22.setHeightForWidth(n2_purge_button->sizePolicy().hasHeightForWidth());
        n2_purge_button->setSizePolicy(sizePolicy22);
        n2_purge_button->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: gray;\n"
"font: 14pt \"Ubuntu Bold\";\n"
"}"));
        n2_purge_button->setCheckable(true);

        gridLayout_8->addWidget(n2_purge_button, 4, 0, 1, 1);

        QIcon icon6;
        iconThemeName = QString::fromUtf8("applications-office");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon6 = QIcon::fromTheme(iconThemeName);
        } else {
            icon6.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        tabWidget->addTab(Dashboard, icon6, QString());
        recipe = new QWidget();
        recipe->setObjectName(QString::fromUtf8("recipe"));
        groupBox = new QGroupBox(recipe);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(60, 40, 591, 501));
        gridLayoutWidget_13 = new QWidget(groupBox);
        gridLayoutWidget_13->setObjectName(QString::fromUtf8("gridLayoutWidget_13"));
        gridLayoutWidget_13->setGeometry(QRect(410, 140, 151, 231));
        gridLayout_10 = new QGridLayout(gridLayoutWidget_13);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        gridLayout_10->setContentsMargins(0, 0, 0, 0);
        remove_cascade_recipe_button = new QPushButton(gridLayoutWidget_13);
        remove_cascade_recipe_button->setObjectName(QString::fromUtf8("remove_cascade_recipe_button"));
        sizePolicy20.setHeightForWidth(remove_cascade_recipe_button->sizePolicy().hasHeightForWidth());
        remove_cascade_recipe_button->setSizePolicy(sizePolicy20);

        gridLayout_10->addWidget(remove_cascade_recipe_button, 1, 0, 1, 1);

        add_cascade_recipe_button = new QPushButton(gridLayoutWidget_13);
        add_cascade_recipe_button->setObjectName(QString::fromUtf8("add_cascade_recipe_button"));
        sizePolicy20.setHeightForWidth(add_cascade_recipe_button->sizePolicy().hasHeightForWidth());
        add_cascade_recipe_button->setSizePolicy(sizePolicy20);

        gridLayout_10->addWidget(add_cascade_recipe_button, 0, 0, 1, 1);

        clear_cascade_recipe_button = new QPushButton(gridLayoutWidget_13);
        clear_cascade_recipe_button->setObjectName(QString::fromUtf8("clear_cascade_recipe_button"));
        sizePolicy20.setHeightForWidth(clear_cascade_recipe_button->sizePolicy().hasHeightForWidth());
        clear_cascade_recipe_button->setSizePolicy(sizePolicy20);

        gridLayout_10->addWidget(clear_cascade_recipe_button, 2, 0, 1, 1);

        cascadeList = new QListWidget(groupBox);
        cascadeList->setObjectName(QString::fromUtf8("cascadeList"));
        cascadeList->setGeometry(QRect(20, 140, 371, 231));
        cascade_recipe_name = new QTextEdit(groupBox);
        cascade_recipe_name->setObjectName(QString::fromUtf8("cascade_recipe_name"));
        cascade_recipe_name->setGeometry(QRect(20, 40, 371, 35));
        sizePolicy14.setHeightForWidth(cascade_recipe_name->sizePolicy().hasHeightForWidth());
        cascade_recipe_name->setSizePolicy(sizePolicy14);
        cascade_recipe_name->setMaximumSize(QSize(16777215, 35));
        cascade_recipe_name->setStyleSheet(QString::fromUtf8("font: 16px;"));
        cascade_recipe_name->setReadOnly(true);
        load_cascade_recipe_button = new QPushButton(groupBox);
        load_cascade_recipe_button->setObjectName(QString::fromUtf8("load_cascade_recipe_button"));
        load_cascade_recipe_button->setGeometry(QRect(20, 80, 371, 38));
        load_cascade_recipe_button->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"color: white;\n"
"background-color: rgb(119, 136, 153);\n"
"font: 14pt \"Ubuntu Bold\";\n"
"}\n"
""));
        load_cascade_recipe_button->setIcon(icon5);
        load_cascade_recipe_button->setIconSize(QSize(32, 32));
        save_cascade_recipe_button = new QPushButton(groupBox);
        save_cascade_recipe_button->setObjectName(QString::fromUtf8("save_cascade_recipe_button"));
        save_cascade_recipe_button->setGeometry(QRect(20, 380, 361, 51));
        sizePolicy20.setHeightForWidth(save_cascade_recipe_button->sizePolicy().hasHeightForWidth());
        save_cascade_recipe_button->setSizePolicy(sizePolicy20);
        save_cascade_recipe_button->setStyleSheet(QString::fromUtf8("\n"
"QPushButton {\n"
"color: white;\n"
"background-color: green;\n"
"font: 14pt \"Ubuntu Bold\";\n"
"}"));
        save_cascade_recipe_button->setIcon(icon4);
        QIcon icon7;
        iconThemeName = QString::fromUtf8("folder");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon7 = QIcon::fromTheme(iconThemeName);
        } else {
            icon7.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        tabWidget->addTab(recipe, icon7, QString());
        plasma = new QWidget();
        plasma->setObjectName(QString::fromUtf8("plasma"));
        gridLayoutWidget_3 = new QWidget(plasma);
        gridLayoutWidget_3->setObjectName(QString::fromUtf8("gridLayoutWidget_3"));
        gridLayoutWidget_3->setGeometry(QRect(20, 20, 1608, 689));
        gridLayout_3 = new QGridLayout(gridLayoutWidget_3);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        matchbox_layout = new QHBoxLayout();
        matchbox_layout->setObjectName(QString::fromUtf8("matchbox_layout"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        MB_Layout = new QVBoxLayout();
        MB_Layout->setObjectName(QString::fromUtf8("MB_Layout"));

        verticalLayout_4->addLayout(MB_Layout);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));

        verticalLayout_4->addLayout(horizontalLayout_11);


        matchbox_layout->addLayout(verticalLayout_4);


        gridLayout_3->addLayout(matchbox_layout, 0, 5, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        Gasses_Layout = new QHBoxLayout();
        Gasses_Layout->setSpacing(10);
        Gasses_Layout->setObjectName(QString::fromUtf8("Gasses_Layout"));
        Gas1_Layout = new QVBoxLayout();
        Gas1_Layout->setObjectName(QString::fromUtf8("Gas1_Layout"));
        Gas1_Layout->setContentsMargins(-1, -1, -1, 0);
        gas1_label_dup = new QLabel(gridLayoutWidget_3);
        gas1_label_dup->setObjectName(QString::fromUtf8("gas1_label_dup"));
        gas1_label_dup->setAlignment(Qt::AlignCenter);

        Gas1_Layout->addWidget(gas1_label_dup);

        gas1_progress_slider_Layout = new QHBoxLayout();
        gas1_progress_slider_Layout->setObjectName(QString::fromUtf8("gas1_progress_slider_Layout"));
        gas1_sliderbar_dup = new QSlider(gridLayoutWidget_3);
        gas1_sliderbar_dup->setObjectName(QString::fromUtf8("gas1_sliderbar_dup"));
        gas1_sliderbar_dup->setMaximum(2000);
        gas1_sliderbar_dup->setValue(0);
        gas1_sliderbar_dup->setOrientation(Qt::Vertical);

        gas1_progress_slider_Layout->addWidget(gas1_sliderbar_dup);

        progressBar_dup = new QProgressBar(gridLayoutWidget_3);
        progressBar_dup->setObjectName(QString::fromUtf8("progressBar_dup"));
        sizePolicy19.setHeightForWidth(progressBar_dup->sizePolicy().hasHeightForWidth());
        progressBar_dup->setSizePolicy(sizePolicy19);
        progressBar_dup->setMinimumSize(QSize(55, 0));
        progressBar_dup->setMaximum(20);
        progressBar_dup->setValue(0);
        progressBar_dup->setTextVisible(false);
        progressBar_dup->setOrientation(Qt::Vertical);

        gas1_progress_slider_Layout->addWidget(progressBar_dup);


        Gas1_Layout->addLayout(gas1_progress_slider_Layout);

        gas1_button_readout_layout = new QHBoxLayout();
        gas1_button_readout_layout->setSpacing(0);
        gas1_button_readout_layout->setObjectName(QString::fromUtf8("gas1_button_readout_layout"));
        gas1_button_readout_layout->setContentsMargins(-1, -1, 0, 0);
        gas1_setpoint_button_dup = new QPushButton(gridLayoutWidget_3);
        gas1_setpoint_button_dup->setObjectName(QString::fromUtf8("gas1_setpoint_button_dup"));
        sizePolicy20.setHeightForWidth(gas1_setpoint_button_dup->sizePolicy().hasHeightForWidth());
        gas1_setpoint_button_dup->setSizePolicy(sizePolicy20);
        gas1_setpoint_button_dup->setStyleSheet(QString::fromUtf8("color: black;"));

        gas1_button_readout_layout->addWidget(gas1_setpoint_button_dup);

        gas1_recipe_actual_SLPM = new QVBoxLayout();
        gas1_recipe_actual_SLPM->setSpacing(0);
        gas1_recipe_actual_SLPM->setObjectName(QString::fromUtf8("gas1_recipe_actual_SLPM"));
        gas1_recipe_actual_SLPM->setContentsMargins(-1, -1, 0, -1);
        gas1_actual_SLPM_dup = new QLabel(gridLayoutWidget_3);
        gas1_actual_SLPM_dup->setObjectName(QString::fromUtf8("gas1_actual_SLPM_dup"));
        gas1_actual_SLPM_dup->setFont(font2);
        gas1_actual_SLPM_dup->setStyleSheet(QString::fromUtf8("QLabel {\n"
"color: red;\n"
"font: 16pt \"Arial Bold\";\n"
"}"));
        gas1_actual_SLPM_dup->setFrameShape(QFrame::NoFrame);
        gas1_actual_SLPM_dup->setFrameShadow(QFrame::Plain);
        gas1_actual_SLPM_dup->setTextFormat(Qt::AutoText);
        gas1_actual_SLPM_dup->setScaledContents(true);
        gas1_actual_SLPM_dup->setAlignment(Qt::AlignCenter);
        gas1_actual_SLPM_dup->setTextInteractionFlags(Qt::NoTextInteraction);

        gas1_recipe_actual_SLPM->addWidget(gas1_actual_SLPM_dup);

        gas1_recipe_SLPM_dup = new QLabel(gridLayoutWidget_3);
        gas1_recipe_SLPM_dup->setObjectName(QString::fromUtf8("gas1_recipe_SLPM_dup"));
        sizePolicy20.setHeightForWidth(gas1_recipe_SLPM_dup->sizePolicy().hasHeightForWidth());
        gas1_recipe_SLPM_dup->setSizePolicy(sizePolicy20);
        gas1_recipe_SLPM_dup->setMinimumSize(QSize(75, 0));
        gas1_recipe_SLPM_dup->setFont(font2);
        gas1_recipe_SLPM_dup->setStyleSheet(QString::fromUtf8("QLabel {\n"
"color: #bfd5ed;\n"
"font: 16pt \"Arial Bold\";\n"
"}"));
        gas1_recipe_SLPM_dup->setFrameShape(QFrame::NoFrame);
        gas1_recipe_SLPM_dup->setFrameShadow(QFrame::Plain);
        gas1_recipe_SLPM_dup->setTextFormat(Qt::AutoText);
        gas1_recipe_SLPM_dup->setScaledContents(true);
        gas1_recipe_SLPM_dup->setAlignment(Qt::AlignCenter);
        gas1_recipe_SLPM_dup->setTextInteractionFlags(Qt::NoTextInteraction);

        gas1_recipe_actual_SLPM->addWidget(gas1_recipe_SLPM_dup);


        gas1_button_readout_layout->addLayout(gas1_recipe_actual_SLPM);


        Gas1_Layout->addLayout(gas1_button_readout_layout);


        Gasses_Layout->addLayout(Gas1_Layout);

        Gas2_Layout = new QVBoxLayout();
        Gas2_Layout->setObjectName(QString::fromUtf8("Gas2_Layout"));
        Gas2_Layout->setContentsMargins(-1, -1, -1, 0);
        gas2_label_dup = new QLabel(gridLayoutWidget_3);
        gas2_label_dup->setObjectName(QString::fromUtf8("gas2_label_dup"));
        gas2_label_dup->setAlignment(Qt::AlignCenter);

        Gas2_Layout->addWidget(gas2_label_dup);

        gas2_progress_slider_layout = new QHBoxLayout();
        gas2_progress_slider_layout->setObjectName(QString::fromUtf8("gas2_progress_slider_layout"));
        gas2_sliderbar_dup = new QSlider(gridLayoutWidget_3);
        gas2_sliderbar_dup->setObjectName(QString::fromUtf8("gas2_sliderbar_dup"));
        gas2_sliderbar_dup->setMaximum(2000);
        gas2_sliderbar_dup->setOrientation(Qt::Vertical);

        gas2_progress_slider_layout->addWidget(gas2_sliderbar_dup);

        gas2_progressbar_dup = new QProgressBar(gridLayoutWidget_3);
        gas2_progressbar_dup->setObjectName(QString::fromUtf8("gas2_progressbar_dup"));
        sizePolicy19.setHeightForWidth(gas2_progressbar_dup->sizePolicy().hasHeightForWidth());
        gas2_progressbar_dup->setSizePolicy(sizePolicy19);
        gas2_progressbar_dup->setMinimumSize(QSize(55, 0));
        gas2_progressbar_dup->setMaximum(20);
        gas2_progressbar_dup->setValue(0);
        gas2_progressbar_dup->setTextVisible(false);
        gas2_progressbar_dup->setOrientation(Qt::Vertical);

        gas2_progress_slider_layout->addWidget(gas2_progressbar_dup);


        Gas2_Layout->addLayout(gas2_progress_slider_layout);

        gas2_buttton_readout_layout = new QHBoxLayout();
        gas2_buttton_readout_layout->setObjectName(QString::fromUtf8("gas2_buttton_readout_layout"));
        gas2_setpoint_button_dup = new QPushButton(gridLayoutWidget_3);
        gas2_setpoint_button_dup->setObjectName(QString::fromUtf8("gas2_setpoint_button_dup"));
        sizePolicy20.setHeightForWidth(gas2_setpoint_button_dup->sizePolicy().hasHeightForWidth());
        gas2_setpoint_button_dup->setSizePolicy(sizePolicy20);
        gas2_setpoint_button_dup->setStyleSheet(QString::fromUtf8("color: black;"));

        gas2_buttton_readout_layout->addWidget(gas2_setpoint_button_dup);

        gas2_recipe_actual_SLPM = new QVBoxLayout();
        gas2_recipe_actual_SLPM->setSpacing(0);
        gas2_recipe_actual_SLPM->setObjectName(QString::fromUtf8("gas2_recipe_actual_SLPM"));
        gas2_actual_SLPM_dup = new QLabel(gridLayoutWidget_3);
        gas2_actual_SLPM_dup->setObjectName(QString::fromUtf8("gas2_actual_SLPM_dup"));
        gas2_actual_SLPM_dup->setFont(font2);
        gas2_actual_SLPM_dup->setStyleSheet(QString::fromUtf8("QLabel {\n"
"color: red;\n"
"font: 16pt \"Arial Bold\";\n"
"}"));
        gas2_actual_SLPM_dup->setFrameShape(QFrame::NoFrame);
        gas2_actual_SLPM_dup->setFrameShadow(QFrame::Plain);
        gas2_actual_SLPM_dup->setTextFormat(Qt::AutoText);
        gas2_actual_SLPM_dup->setScaledContents(true);
        gas2_actual_SLPM_dup->setAlignment(Qt::AlignCenter);
        gas2_actual_SLPM_dup->setTextInteractionFlags(Qt::NoTextInteraction);

        gas2_recipe_actual_SLPM->addWidget(gas2_actual_SLPM_dup);

        gas2_recipe_SLPM_dup = new QLabel(gridLayoutWidget_3);
        gas2_recipe_SLPM_dup->setObjectName(QString::fromUtf8("gas2_recipe_SLPM_dup"));
        sizePolicy20.setHeightForWidth(gas2_recipe_SLPM_dup->sizePolicy().hasHeightForWidth());
        gas2_recipe_SLPM_dup->setSizePolicy(sizePolicy20);
        gas2_recipe_SLPM_dup->setMinimumSize(QSize(75, 0));
        gas2_recipe_SLPM_dup->setFont(font2);
        gas2_recipe_SLPM_dup->setStyleSheet(QString::fromUtf8("QLabel {\n"
"color: #bfd5ed;\n"
"font: 16pt \"Arial Bold\";\n"
"}"));
        gas2_recipe_SLPM_dup->setFrameShape(QFrame::NoFrame);
        gas2_recipe_SLPM_dup->setFrameShadow(QFrame::Plain);
        gas2_recipe_SLPM_dup->setTextFormat(Qt::AutoText);
        gas2_recipe_SLPM_dup->setScaledContents(true);
        gas2_recipe_SLPM_dup->setAlignment(Qt::AlignCenter);
        gas2_recipe_SLPM_dup->setTextInteractionFlags(Qt::NoTextInteraction);

        gas2_recipe_actual_SLPM->addWidget(gas2_recipe_SLPM_dup);


        gas2_buttton_readout_layout->addLayout(gas2_recipe_actual_SLPM);


        Gas2_Layout->addLayout(gas2_buttton_readout_layout);


        Gasses_Layout->addLayout(Gas2_Layout);

        Gas3_Layout = new QVBoxLayout();
        Gas3_Layout->setObjectName(QString::fromUtf8("Gas3_Layout"));
        Gas3_Layout->setContentsMargins(-1, -1, -1, 0);
        gas3_label_dup = new QLabel(gridLayoutWidget_3);
        gas3_label_dup->setObjectName(QString::fromUtf8("gas3_label_dup"));
        gas3_label_dup->setAlignment(Qt::AlignCenter);

        Gas3_Layout->addWidget(gas3_label_dup);

        horizontalLayout_23 = new QHBoxLayout();
        horizontalLayout_23->setObjectName(QString::fromUtf8("horizontalLayout_23"));
        gas3_sliderbar_dup = new QSlider(gridLayoutWidget_3);
        gas3_sliderbar_dup->setObjectName(QString::fromUtf8("gas3_sliderbar_dup"));
        gas3_sliderbar_dup->setMaximum(30);
        gas3_sliderbar_dup->setOrientation(Qt::Vertical);

        horizontalLayout_23->addWidget(gas3_sliderbar_dup);

        gas3_progessbar_dup = new QProgressBar(gridLayoutWidget_3);
        gas3_progessbar_dup->setObjectName(QString::fromUtf8("gas3_progessbar_dup"));
        sizePolicy19.setHeightForWidth(gas3_progessbar_dup->sizePolicy().hasHeightForWidth());
        gas3_progessbar_dup->setSizePolicy(sizePolicy19);
        gas3_progessbar_dup->setMinimumSize(QSize(55, 0));
        gas3_progessbar_dup->setMaximum(30);
        gas3_progessbar_dup->setValue(0);
        gas3_progessbar_dup->setTextVisible(false);
        gas3_progessbar_dup->setOrientation(Qt::Vertical);

        horizontalLayout_23->addWidget(gas3_progessbar_dup);


        Gas3_Layout->addLayout(horizontalLayout_23);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        gas3_setpoint_button_dup = new QPushButton(gridLayoutWidget_3);
        gas3_setpoint_button_dup->setObjectName(QString::fromUtf8("gas3_setpoint_button_dup"));
        sizePolicy20.setHeightForWidth(gas3_setpoint_button_dup->sizePolicy().hasHeightForWidth());
        gas3_setpoint_button_dup->setSizePolicy(sizePolicy20);
        gas3_setpoint_button_dup->setStyleSheet(QString::fromUtf8("color: black;"));

        horizontalLayout_8->addWidget(gas3_setpoint_button_dup);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(0);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        gas3_actual_SLPM_dup = new QLabel(gridLayoutWidget_3);
        gas3_actual_SLPM_dup->setObjectName(QString::fromUtf8("gas3_actual_SLPM_dup"));
        gas3_actual_SLPM_dup->setFont(font2);
        gas3_actual_SLPM_dup->setStyleSheet(QString::fromUtf8("QLabel {\n"
"color: red;\n"
"font: 16pt \"Arial Bold\";\n"
"}"));
        gas3_actual_SLPM_dup->setFrameShape(QFrame::NoFrame);
        gas3_actual_SLPM_dup->setFrameShadow(QFrame::Plain);
        gas3_actual_SLPM_dup->setTextFormat(Qt::AutoText);
        gas3_actual_SLPM_dup->setScaledContents(true);
        gas3_actual_SLPM_dup->setAlignment(Qt::AlignCenter);
        gas3_actual_SLPM_dup->setTextInteractionFlags(Qt::NoTextInteraction);

        verticalLayout_6->addWidget(gas3_actual_SLPM_dup);

        gas3_recipe_SLPM_dup = new QLabel(gridLayoutWidget_3);
        gas3_recipe_SLPM_dup->setObjectName(QString::fromUtf8("gas3_recipe_SLPM_dup"));
        sizePolicy20.setHeightForWidth(gas3_recipe_SLPM_dup->sizePolicy().hasHeightForWidth());
        gas3_recipe_SLPM_dup->setSizePolicy(sizePolicy20);
        gas3_recipe_SLPM_dup->setMinimumSize(QSize(75, 0));
        gas3_recipe_SLPM_dup->setFont(font2);
        gas3_recipe_SLPM_dup->setStyleSheet(QString::fromUtf8("QLabel {\n"
"color: #bfd5ed;\n"
"font: 16pt \"Arial Bold\";\n"
"}"));
        gas3_recipe_SLPM_dup->setFrameShape(QFrame::NoFrame);
        gas3_recipe_SLPM_dup->setFrameShadow(QFrame::Plain);
        gas3_recipe_SLPM_dup->setTextFormat(Qt::AutoText);
        gas3_recipe_SLPM_dup->setScaledContents(true);
        gas3_recipe_SLPM_dup->setAlignment(Qt::AlignCenter);
        gas3_recipe_SLPM_dup->setTextInteractionFlags(Qt::NoTextInteraction);

        verticalLayout_6->addWidget(gas3_recipe_SLPM_dup);


        horizontalLayout_8->addLayout(verticalLayout_6);


        Gas3_Layout->addLayout(horizontalLayout_8);


        Gasses_Layout->addLayout(Gas3_Layout);

        Gas4_Layout = new QVBoxLayout();
        Gas4_Layout->setObjectName(QString::fromUtf8("Gas4_Layout"));
        Gas4_Layout->setContentsMargins(-1, -1, -1, 0);
        gas4_label_dup = new QLabel(gridLayoutWidget_3);
        gas4_label_dup->setObjectName(QString::fromUtf8("gas4_label_dup"));
        gas4_label_dup->setAlignment(Qt::AlignCenter);

        Gas4_Layout->addWidget(gas4_label_dup);

        horizontalLayout_24 = new QHBoxLayout();
        horizontalLayout_24->setObjectName(QString::fromUtf8("horizontalLayout_24"));
        gas4_sliderbar_dup = new QSlider(gridLayoutWidget_3);
        gas4_sliderbar_dup->setObjectName(QString::fromUtf8("gas4_sliderbar_dup"));
        gas4_sliderbar_dup->setMinimum(0);
        gas4_sliderbar_dup->setMaximum(30);
        gas4_sliderbar_dup->setOrientation(Qt::Vertical);
        gas4_sliderbar_dup->setTickInterval(1);

        horizontalLayout_24->addWidget(gas4_sliderbar_dup);

        gas4_progressbar_dup = new QProgressBar(gridLayoutWidget_3);
        gas4_progressbar_dup->setObjectName(QString::fromUtf8("gas4_progressbar_dup"));
        sizePolicy19.setHeightForWidth(gas4_progressbar_dup->sizePolicy().hasHeightForWidth());
        gas4_progressbar_dup->setSizePolicy(sizePolicy19);
        gas4_progressbar_dup->setMinimumSize(QSize(55, 0));
        gas4_progressbar_dup->setMaximum(30);
        gas4_progressbar_dup->setValue(0);
        gas4_progressbar_dup->setTextVisible(false);
        gas4_progressbar_dup->setOrientation(Qt::Vertical);

        horizontalLayout_24->addWidget(gas4_progressbar_dup);


        Gas4_Layout->addLayout(horizontalLayout_24);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        gas4_setpoint_button_dup = new QPushButton(gridLayoutWidget_3);
        gas4_setpoint_button_dup->setObjectName(QString::fromUtf8("gas4_setpoint_button_dup"));
        sizePolicy20.setHeightForWidth(gas4_setpoint_button_dup->sizePolicy().hasHeightForWidth());
        gas4_setpoint_button_dup->setSizePolicy(sizePolicy20);
        gas4_setpoint_button_dup->setStyleSheet(QString::fromUtf8("color: black;"));

        horizontalLayout_9->addWidget(gas4_setpoint_button_dup);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(0);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        gas4_actual_SLPM_dup = new QLabel(gridLayoutWidget_3);
        gas4_actual_SLPM_dup->setObjectName(QString::fromUtf8("gas4_actual_SLPM_dup"));
        gas4_actual_SLPM_dup->setFont(font2);
        gas4_actual_SLPM_dup->setStyleSheet(QString::fromUtf8("QLabel {\n"
"color: red;\n"
"font: 16pt \"Arial Bold\";\n"
"}"));
        gas4_actual_SLPM_dup->setFrameShape(QFrame::NoFrame);
        gas4_actual_SLPM_dup->setFrameShadow(QFrame::Plain);
        gas4_actual_SLPM_dup->setTextFormat(Qt::AutoText);
        gas4_actual_SLPM_dup->setScaledContents(true);
        gas4_actual_SLPM_dup->setAlignment(Qt::AlignCenter);
        gas4_actual_SLPM_dup->setTextInteractionFlags(Qt::NoTextInteraction);

        verticalLayout_8->addWidget(gas4_actual_SLPM_dup);

        gas4_recipe_SLPM_dup = new QLabel(gridLayoutWidget_3);
        gas4_recipe_SLPM_dup->setObjectName(QString::fromUtf8("gas4_recipe_SLPM_dup"));
        sizePolicy20.setHeightForWidth(gas4_recipe_SLPM_dup->sizePolicy().hasHeightForWidth());
        gas4_recipe_SLPM_dup->setSizePolicy(sizePolicy20);
        gas4_recipe_SLPM_dup->setMinimumSize(QSize(75, 0));
        gas4_recipe_SLPM_dup->setFont(font2);
        gas4_recipe_SLPM_dup->setStyleSheet(QString::fromUtf8("QLabel {\n"
"color: #bfd5ed;\n"
"font: 16pt \"Arial Bold\";\n"
"}"));
        gas4_recipe_SLPM_dup->setFrameShape(QFrame::NoFrame);
        gas4_recipe_SLPM_dup->setFrameShadow(QFrame::Plain);
        gas4_recipe_SLPM_dup->setTextFormat(Qt::AutoText);
        gas4_recipe_SLPM_dup->setScaledContents(true);
        gas4_recipe_SLPM_dup->setAlignment(Qt::AlignCenter);
        gas4_recipe_SLPM_dup->setTextInteractionFlags(Qt::NoTextInteraction);

        verticalLayout_8->addWidget(gas4_recipe_SLPM_dup);


        horizontalLayout_9->addLayout(verticalLayout_8);


        Gas4_Layout->addLayout(horizontalLayout_9);


        Gasses_Layout->addLayout(Gas4_Layout);


        horizontalLayout_6->addLayout(Gasses_Layout);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        horizontalLayout_6->addItem(verticalSpacer_2);

        Power_Layout = new QHBoxLayout();
        Power_Layout->setSpacing(6);
        Power_Layout->setObjectName(QString::fromUtf8("Power_Layout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        RF_Actual_LCD_dup = new QLCDNumber(gridLayoutWidget_3);
        RF_Actual_LCD_dup->setObjectName(QString::fromUtf8("RF_Actual_LCD_dup"));
        QSizePolicy sizePolicy24(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy24.setHorizontalStretch(0);
        sizePolicy24.setVerticalStretch(0);
        sizePolicy24.setHeightForWidth(RF_Actual_LCD_dup->sizePolicy().hasHeightForWidth());
        RF_Actual_LCD_dup->setSizePolicy(sizePolicy24);
        RF_Actual_LCD_dup->setMinimumSize(QSize(150, 75));
        RF_Actual_LCD_dup->setStyleSheet(QString::fromUtf8("color: rgb(204, 0, 0);"));
        RF_Actual_LCD_dup->setFrameShape(QFrame::Box);
        RF_Actual_LCD_dup->setFrameShadow(QFrame::Raised);
        RF_Actual_LCD_dup->setDigitCount(3);
        RF_Actual_LCD_dup->setProperty("value", QVariant(0.000000000000000));
        RF_Actual_LCD_dup->setProperty("intValue", QVariant(0));

        horizontalLayout_4->addWidget(RF_Actual_LCD_dup);

        RFfor_label = new QLabel(gridLayoutWidget_3);
        RFfor_label->setObjectName(QString::fromUtf8("RFfor_label"));
        RFfor_label->setStyleSheet(QString::fromUtf8("font: 18pt \"Ubuntu\";"));
        RFfor_label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(RFfor_label);


        verticalLayout->addLayout(horizontalLayout_4);

        RF_bar_dup = new QProgressBar(gridLayoutWidget_3);
        RF_bar_dup->setObjectName(QString::fromUtf8("RF_bar_dup"));
        RF_bar_dup->setMinimumSize(QSize(0, 55));
        RF_bar_dup->setMaximum(200);
        RF_bar_dup->setValue(0);
        RF_bar_dup->setTextVisible(false);
        RF_bar_dup->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(RF_bar_dup);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        MB_Actual_LCD_dup = new QLCDNumber(gridLayoutWidget_3);
        MB_Actual_LCD_dup->setObjectName(QString::fromUtf8("MB_Actual_LCD_dup"));
        sizePolicy24.setHeightForWidth(MB_Actual_LCD_dup->sizePolicy().hasHeightForWidth());
        MB_Actual_LCD_dup->setSizePolicy(sizePolicy24);
        MB_Actual_LCD_dup->setMinimumSize(QSize(150, 75));
        MB_Actual_LCD_dup->setStyleSheet(QString::fromUtf8("color: black;"));
        MB_Actual_LCD_dup->setFrameShape(QFrame::Box);
        MB_Actual_LCD_dup->setFrameShadow(QFrame::Raised);
        MB_Actual_LCD_dup->setDigitCount(3);
        MB_Actual_LCD_dup->setProperty("intValue", QVariant(0));

        horizontalLayout_7->addWidget(MB_Actual_LCD_dup);

        MBfor_label_2 = new QLabel(gridLayoutWidget_3);
        MBfor_label_2->setObjectName(QString::fromUtf8("MBfor_label_2"));
        MBfor_label_2->setStyleSheet(QString::fromUtf8("font: 18pt \"Ubuntu\";"));
        MBfor_label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_7->addWidget(MBfor_label_2);


        verticalLayout->addLayout(horizontalLayout_7);

        MB_Pos_Bar_dup = new QProgressBar(gridLayoutWidget_3);
        MB_Pos_Bar_dup->setObjectName(QString::fromUtf8("MB_Pos_Bar_dup"));
        sizePolicy17.setHeightForWidth(MB_Pos_Bar_dup->sizePolicy().hasHeightForWidth());
        MB_Pos_Bar_dup->setSizePolicy(sizePolicy17);
        MB_Pos_Bar_dup->setMinimumSize(QSize(0, 55));
        MB_Pos_Bar_dup->setStyleSheet(QString::fromUtf8("QProgressBar::Chunk {\n"
"background-color: black;\n"
"}"));
        MB_Pos_Bar_dup->setValue(50);
        MB_Pos_Bar_dup->setTextVisible(false);
        MB_Pos_Bar_dup->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(MB_Pos_Bar_dup);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        RefRF_Actual_LCD_dup = new QLCDNumber(gridLayoutWidget_3);
        RefRF_Actual_LCD_dup->setObjectName(QString::fromUtf8("RefRF_Actual_LCD_dup"));
        sizePolicy24.setHeightForWidth(RefRF_Actual_LCD_dup->sizePolicy().hasHeightForWidth());
        RefRF_Actual_LCD_dup->setSizePolicy(sizePolicy24);
        RefRF_Actual_LCD_dup->setMinimumSize(QSize(150, 75));
        RefRF_Actual_LCD_dup->setStyleSheet(QString::fromUtf8("color:  rgb(255, 242, 0);\n"
""));
        RefRF_Actual_LCD_dup->setFrameShape(QFrame::Box);
        RefRF_Actual_LCD_dup->setFrameShadow(QFrame::Raised);
        RefRF_Actual_LCD_dup->setDigitCount(3);
        RefRF_Actual_LCD_dup->setProperty("intValue", QVariant(30));

        horizontalLayout_10->addWidget(RefRF_Actual_LCD_dup);

        ref_label = new QLabel(gridLayoutWidget_3);
        ref_label->setObjectName(QString::fromUtf8("ref_label"));
        ref_label->setStyleSheet(QString::fromUtf8("font: 18pt \"Ubuntu\";"));
        ref_label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_10->addWidget(ref_label);


        verticalLayout->addLayout(horizontalLayout_10);

        RefRF_bar_dup = new QProgressBar(gridLayoutWidget_3);
        RefRF_bar_dup->setObjectName(QString::fromUtf8("RefRF_bar_dup"));
        sizePolicy17.setHeightForWidth(RefRF_bar_dup->sizePolicy().hasHeightForWidth());
        RefRF_bar_dup->setSizePolicy(sizePolicy17);
        RefRF_bar_dup->setMinimumSize(QSize(0, 55));
        RefRF_bar_dup->setStyleSheet(QString::fromUtf8("QProgressBar \n"
"{		\n"
"	border-radius: 26px;\n"
"	background-color: #d3dbdd;\n"
"}\n"
"\n"
"QProgressBar::Chunk\n"
"{\n"
"    border-radius: 26px;\n"
"	background-color:  rgb(255, 242, 0);\n"
"}\n"
""));
        RefRF_bar_dup->setValue(24);
        RefRF_bar_dup->setTextVisible(false);
        RefRF_bar_dup->setOrientation(Qt::Horizontal);
        RefRF_bar_dup->setInvertedAppearance(true);

        verticalLayout->addWidget(RefRF_bar_dup);

        RF_Dials = new QVBoxLayout();
        RF_Dials->setObjectName(QString::fromUtf8("RF_Dials"));
        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        temp_LCD_dup = new QLCDNumber(gridLayoutWidget_3);
        temp_LCD_dup->setObjectName(QString::fromUtf8("temp_LCD_dup"));
        sizePolicy24.setHeightForWidth(temp_LCD_dup->sizePolicy().hasHeightForWidth());
        temp_LCD_dup->setSizePolicy(sizePolicy24);
        temp_LCD_dup->setMinimumSize(QSize(150, 75));
        temp_LCD_dup->setStyleSheet(QString::fromUtf8("color: blue;"));
        temp_LCD_dup->setFrameShape(QFrame::Box);
        temp_LCD_dup->setFrameShadow(QFrame::Raised);
        temp_LCD_dup->setDigitCount(3);
        temp_LCD_dup->setProperty("intValue", QVariant(0));

        horizontalLayout_12->addWidget(temp_LCD_dup);

        label_20 = new QLabel(gridLayoutWidget_3);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setStyleSheet(QString::fromUtf8("font: 18pt \"Ubuntu\";"));
        label_20->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_12->addWidget(label_20);

        ref_label_2 = new QLabel(gridLayoutWidget_3);
        ref_label_2->setObjectName(QString::fromUtf8("ref_label_2"));
        ref_label_2->setStyleSheet(QString::fromUtf8("font: 18pt \"Ubuntu\";"));
        ref_label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_12->addWidget(ref_label_2);


        verticalLayout_9->addLayout(horizontalLayout_12);

        Temp_bar_dup = new QProgressBar(gridLayoutWidget_3);
        Temp_bar_dup->setObjectName(QString::fromUtf8("Temp_bar_dup"));
        sizePolicy17.setHeightForWidth(Temp_bar_dup->sizePolicy().hasHeightForWidth());
        Temp_bar_dup->setSizePolicy(sizePolicy17);
        Temp_bar_dup->setMinimumSize(QSize(0, 55));
        Temp_bar_dup->setStyleSheet(QString::fromUtf8("QProgressBar::Chunk {\n"
"background-color: blue;\n"
"}"));
        Temp_bar_dup->setValue(50);
        Temp_bar_dup->setTextVisible(false);
        Temp_bar_dup->setOrientation(Qt::Horizontal);

        verticalLayout_9->addWidget(Temp_bar_dup);

        horizontalLayout_26 = new QHBoxLayout();
        horizontalLayout_26->setObjectName(QString::fromUtf8("horizontalLayout_26"));

        verticalLayout_9->addLayout(horizontalLayout_26);


        horizontalLayout_15->addLayout(verticalLayout_9);


        RF_Dials->addLayout(horizontalLayout_15);


        verticalLayout->addLayout(RF_Dials);


        Power_Layout->addLayout(verticalLayout);


        horizontalLayout_6->addLayout(Power_Layout);


        gridLayout_3->addLayout(horizontalLayout_6, 0, 0, 1, 1);

        verticalLayout_13 = new QVBoxLayout();
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        verticalLayout_13->setSizeConstraint(QLayout::SetMaximumSize);
        RF_Setpoint_Button = new QPushButton(gridLayoutWidget_3);
        RF_Setpoint_Button->setObjectName(QString::fromUtf8("RF_Setpoint_Button"));
        QSizePolicy sizePolicy25(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy25.setHorizontalStretch(0);
        sizePolicy25.setVerticalStretch(0);
        sizePolicy25.setHeightForWidth(RF_Setpoint_Button->sizePolicy().hasHeightForWidth());
        RF_Setpoint_Button->setSizePolicy(sizePolicy25);
        RF_Setpoint_Button->setMinimumSize(QSize(100, 0));
        RF_Setpoint_Button->setMaximumSize(QSize(16777215, 16777215));
        RF_Setpoint_Button->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"color: white;\n"
"background-color: red;\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        verticalLayout_13->addWidget(RF_Setpoint_Button);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_13->addItem(horizontalSpacer_2);

        autoMode_Button_dup = new QPushButton(gridLayoutWidget_3);
        autoMode_Button_dup->setObjectName(QString::fromUtf8("autoMode_Button_dup"));
        sizePolicy13.setHeightForWidth(autoMode_Button_dup->sizePolicy().hasHeightForWidth());
        autoMode_Button_dup->setSizePolicy(sizePolicy13);
        autoMode_Button_dup->setLayoutDirection(Qt::LeftToRight);
        autoMode_Button_dup->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"color: white;\n"
"background-color: black;\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));
        autoMode_Button_dup->setIconSize(QSize(128, 64));

        verticalLayout_13->addWidget(autoMode_Button_dup);

        MB_Setpoint_Button = new QPushButton(gridLayoutWidget_3);
        MB_Setpoint_Button->setObjectName(QString::fromUtf8("MB_Setpoint_Button"));
        sizePolicy25.setHeightForWidth(MB_Setpoint_Button->sizePolicy().hasHeightForWidth());
        MB_Setpoint_Button->setSizePolicy(sizePolicy25);
        MB_Setpoint_Button->setMinimumSize(QSize(100, 0));
        MB_Setpoint_Button->setMaximumSize(QSize(16777215, 16777215));
        MB_Setpoint_Button->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"color: white;\n"
"background-color: black;\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        verticalLayout_13->addWidget(MB_Setpoint_Button);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        MB_Left_Button_dup = new QPushButton(gridLayoutWidget_3);
        MB_Left_Button_dup->setObjectName(QString::fromUtf8("MB_Left_Button_dup"));
        QSizePolicy sizePolicy26(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy26.setHorizontalStretch(0);
        sizePolicy26.setVerticalStretch(0);
        sizePolicy26.setHeightForWidth(MB_Left_Button_dup->sizePolicy().hasHeightForWidth());
        MB_Left_Button_dup->setSizePolicy(sizePolicy26);
        MB_Left_Button_dup->setLayoutDirection(Qt::LeftToRight);
        MB_Left_Button_dup->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"color: white;\n"
"background-color: black;\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));
        MB_Left_Button_dup->setIconSize(QSize(128, 64));

        horizontalLayout->addWidget(MB_Left_Button_dup);

        MB_Right_Button_dup = new QPushButton(gridLayoutWidget_3);
        MB_Right_Button_dup->setObjectName(QString::fromUtf8("MB_Right_Button_dup"));
        sizePolicy26.setHeightForWidth(MB_Right_Button_dup->sizePolicy().hasHeightForWidth());
        MB_Right_Button_dup->setSizePolicy(sizePolicy26);
        MB_Right_Button_dup->setLayoutDirection(Qt::RightToLeft);
        MB_Right_Button_dup->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"color: white;\n"
"background-color: black;\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));
        MB_Right_Button_dup->setIconSize(QSize(128, 64));

        horizontalLayout->addWidget(MB_Right_Button_dup);


        verticalLayout_13->addLayout(horizontalLayout);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_13->addItem(horizontalSpacer_3);

        Heater_Button = new QPushButton(gridLayoutWidget_3);
        Heater_Button->setObjectName(QString::fromUtf8("Heater_Button"));
        sizePolicy20.setHeightForWidth(Heater_Button->sizePolicy().hasHeightForWidth());
        Heater_Button->setSizePolicy(sizePolicy20);
        Heater_Button->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));
        Heater_Button->setCheckable(true);

        verticalLayout_13->addWidget(Heater_Button);


        gridLayout_3->addLayout(verticalLayout_13, 0, 2, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 0, 1, 1, 1);

        plsmaBtn_dup = new QPushButton(gridLayoutWidget_3);
        plsmaBtn_dup->setObjectName(QString::fromUtf8("plsmaBtn_dup"));
        sizePolicy21.setHeightForWidth(plsmaBtn_dup->sizePolicy().hasHeightForWidth());
        plsmaBtn_dup->setSizePolicy(sizePolicy21);
        plsmaBtn_dup->setMinimumSize(QSize(250, 100));
        plsmaBtn_dup->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: blue;\n"
"font: 28pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_3->addWidget(plsmaBtn_dup, 0, 4, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_3, 0, 3, 1, 1);

        Gasses_Label = new QLabel(plasma);
        Gasses_Label->setObjectName(QString::fromUtf8("Gasses_Label"));
        Gasses_Label->setGeometry(QRect(30, 0, 41, 16));
        Send_CMD_button = new QPushButton(plasma);
        Send_CMD_button->setObjectName(QString::fromUtf8("Send_CMD_button"));
        Send_CMD_button->setGeometry(QRect(750, 720, 80, 24));
        Send_CMD_button->setStyleSheet(QString::fromUtf8("color: black;"));
        textRCVbox = new QPlainTextEdit(plasma);
        textRCVbox->setObjectName(QString::fromUtf8("textRCVbox"));
        textRCVbox->setGeometry(QRect(841, 717, 278, 41));
        textRCVbox->setBackgroundVisible(false);
        Clear_Button = new QPushButton(plasma);
        Clear_Button->setObjectName(QString::fromUtf8("Clear_Button"));
        Clear_Button->setGeometry(QRect(380, 720, 80, 24));
        Clear_Button->setStyleSheet(QString::fromUtf8("color: black;"));
        CMDlabel = new QLabel(plasma);
        CMDlabel->setObjectName(QString::fromUtf8("CMDlabel"));
        CMDlabel->setGeometry(QRect(249, 1489, 168, 78));
        sizePolicy13.setHeightForWidth(CMDlabel->sizePolicy().hasHeightForWidth());
        CMDlabel->setSizePolicy(sizePolicy13);
        CMDlabel->setStyleSheet(QString::fromUtf8("font: 12pt \"Ubuntu\";"));
        CMDlabel->setAlignment(Qt::AlignCenter);
        text2sendCMDbox = new QPlainTextEdit(plasma);
        text2sendCMDbox->setObjectName(QString::fromUtf8("text2sendCMDbox"));
        text2sendCMDbox->setGeometry(QRect(470, 717, 279, 41));
        text2sendCMDbox->setBackgroundVisible(false);
        RCVlabel = new QLabel(plasma);
        RCVlabel->setObjectName(QString::fromUtf8("RCVlabel"));
        RCVlabel->setGeometry(QRect(1019, 1489, 142, 78));
        sizePolicy13.setHeightForWidth(RCVlabel->sizePolicy().hasHeightForWidth());
        RCVlabel->setSizePolicy(sizePolicy13);
        RCVlabel->setStyleSheet(QString::fromUtf8("font: 12pt \"Ubuntu\";"));
        RCVlabel->setAlignment(Qt::AlignCenter);
        textCMDbox = new QPlainTextEdit(plasma);
        textCMDbox->setObjectName(QString::fromUtf8("textCMDbox"));
        textCMDbox->setGeometry(QRect(100, 717, 278, 41));
        textCMDbox->setBackgroundVisible(false);
        QIcon icon8;
        iconThemeName = QString::fromUtf8("applications-science");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon8 = QIcon::fromTheme(iconThemeName);
        } else {
            icon8.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        tabWidget->addTab(plasma, icon8, QString());
        axis = new QWidget();
        axis->setObjectName(QString::fromUtf8("axis"));
        label_39 = new QLabel(axis);
        label_39->setObjectName(QString::fromUtf8("label_39"));
        label_39->setGeometry(QRect(80, 360, 111, 20));
        gridLayoutWidget = new QWidget(axis);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(70, 60, 844, 291));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        init_button_dup = new QPushButton(gridLayoutWidget);
        init_button_dup->setObjectName(QString::fromUtf8("init_button_dup"));
        sizePolicy22.setHeightForWidth(init_button_dup->sizePolicy().hasHeightForWidth());
        init_button_dup->setSizePolicy(sizePolicy22);
        init_button_dup->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout->addWidget(init_button_dup, 3, 0, 1, 1);

        Stagepins_button_dup = new QPushButton(gridLayoutWidget);
        Stagepins_button_dup->setObjectName(QString::fromUtf8("Stagepins_button_dup"));
        sizePolicy22.setHeightForWidth(Stagepins_button_dup->sizePolicy().hasHeightForWidth());
        Stagepins_button_dup->setSizePolicy(sizePolicy22);
        Stagepins_button_dup->setStyleSheet(QString::fromUtf8("QPushButton {background-color: gray;\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));
        Stagepins_button_dup->setCheckable(true);

        gridLayout->addWidget(Stagepins_button_dup, 2, 3, 1, 1);

        label_45 = new QLabel(gridLayoutWidget);
        label_45->setObjectName(QString::fromUtf8("label_45"));

        gridLayout->addWidget(label_45, 0, 0, 1, 1);

        diameter_button_dup = new QPushButton(gridLayoutWidget);
        diameter_button_dup->setObjectName(QString::fromUtf8("diameter_button_dup"));
        sizePolicy22.setHeightForWidth(diameter_button_dup->sizePolicy().hasHeightForWidth());
        diameter_button_dup->setSizePolicy(sizePolicy22);
        diameter_button_dup->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout->addWidget(diameter_button_dup, 3, 4, 1, 1);

        Home_button_dup = new QPushButton(gridLayoutWidget);
        Home_button_dup->setObjectName(QString::fromUtf8("Home_button_dup"));
        sizePolicy22.setHeightForWidth(Home_button_dup->sizePolicy().hasHeightForWidth());
        Home_button_dup->setSizePolicy(sizePolicy22);
        Home_button_dup->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout->addWidget(Home_button_dup, 3, 3, 1, 1);

        twospot_button_dup = new QPushButton(gridLayoutWidget);
        twospot_button_dup->setObjectName(QString::fromUtf8("twospot_button_dup"));
        sizePolicy22.setHeightForWidth(twospot_button_dup->sizePolicy().hasHeightForWidth());
        twospot_button_dup->setSizePolicy(sizePolicy22);
        twospot_button_dup->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout->addWidget(twospot_button_dup, 3, 1, 1, 1);

        scan_button_dup = new QPushButton(gridLayoutWidget);
        scan_button_dup->setObjectName(QString::fromUtf8("scan_button_dup"));
        sizePolicy22.setHeightForWidth(scan_button_dup->sizePolicy().hasHeightForWidth());
        scan_button_dup->setSizePolicy(sizePolicy22);
        scan_button_dup->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout->addWidget(scan_button_dup, 3, 2, 1, 1);

        vac_button_dup = new QPushButton(gridLayoutWidget);
        vac_button_dup->setObjectName(QString::fromUtf8("vac_button_dup"));
        sizePolicy22.setHeightForWidth(vac_button_dup->sizePolicy().hasHeightForWidth());
        vac_button_dup->setSizePolicy(sizePolicy22);
        vac_button_dup->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: gray;\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));
        vac_button_dup->setCheckable(true);

        gridLayout->addWidget(vac_button_dup, 2, 1, 1, 1);

        n2_purge_button_dup = new QPushButton(gridLayoutWidget);
        n2_purge_button_dup->setObjectName(QString::fromUtf8("n2_purge_button_dup"));
        sizePolicy22.setHeightForWidth(n2_purge_button_dup->sizePolicy().hasHeightForWidth());
        n2_purge_button_dup->setSizePolicy(sizePolicy22);
        n2_purge_button_dup->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: gray;\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));
        n2_purge_button_dup->setCheckable(true);

        gridLayout->addWidget(n2_purge_button_dup, 2, 2, 1, 1);

        verticalLayout_14 = new QVBoxLayout();
        verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));
        label_33 = new QLabel(gridLayoutWidget);
        label_33->setObjectName(QString::fromUtf8("label_33"));
        sizePolicy23.setHeightForWidth(label_33->sizePolicy().hasHeightForWidth());
        label_33->setSizePolicy(sizePolicy23);
        label_33->setMaximumSize(QSize(16777215, 30));
        label_33->setStyleSheet(QString::fromUtf8("QLabel {\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));
        label_33->setAlignment(Qt::AlignCenter);

        verticalLayout_14->addWidget(label_33);

        wafer_diameter_dup = new QComboBox(gridLayoutWidget);
        wafer_diameter_dup->addItem(QString());
        wafer_diameter_dup->addItem(QString());
        wafer_diameter_dup->addItem(QString());
        wafer_diameter_dup->addItem(QString());
        wafer_diameter_dup->setObjectName(QString::fromUtf8("wafer_diameter_dup"));
        sizePolicy23.setHeightForWidth(wafer_diameter_dup->sizePolicy().hasHeightForWidth());
        wafer_diameter_dup->setSizePolicy(sizePolicy23);
        wafer_diameter_dup->setMaximumSize(QSize(200, 50));
        wafer_diameter_dup->setLayoutDirection(Qt::RightToLeft);
        wafer_diameter_dup->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"background-color: gray;\n"
"font: 16pt \"Ubuntu Bold\";\n"
"}"));

        verticalLayout_14->addWidget(wafer_diameter_dup);


        gridLayout->addLayout(verticalLayout_14, 2, 4, 1, 1);

        Joystick_button_dup = new QPushButton(gridLayoutWidget);
        Joystick_button_dup->setObjectName(QString::fromUtf8("Joystick_button_dup"));
        sizePolicy22.setHeightForWidth(Joystick_button_dup->sizePolicy().hasHeightForWidth());
        Joystick_button_dup->setSizePolicy(sizePolicy22);
        Joystick_button_dup->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: gray;\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));
        Joystick_button_dup->setCheckable(true);

        gridLayout->addWidget(Joystick_button_dup, 2, 0, 1, 1);

        verticalLayoutWidget_7 = new QWidget(axis);
        verticalLayoutWidget_7->setObjectName(QString::fromUtf8("verticalLayoutWidget_7"));
        verticalLayoutWidget_7->setGeometry(QRect(950, 60, 121, 317));
        verticalLayout_10 = new QVBoxLayout(verticalLayoutWidget_7);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        verticalLayout_10->setContentsMargins(0, 0, 0, 0);
        label_34 = new QLabel(verticalLayoutWidget_7);
        label_34->setObjectName(QString::fromUtf8("label_34"));
        label_34->setFont(font);
        label_34->setAlignment(Qt::AlignCenter);

        verticalLayout_10->addWidget(label_34);

        X_relative_PH_dup = new QTextEdit(verticalLayoutWidget_7);
        X_relative_PH_dup->setObjectName(QString::fromUtf8("X_relative_PH_dup"));
        X_relative_PH_dup->setEnabled(true);
        X_relative_PH_dup->setStyleSheet(QString::fromUtf8("font: 28px;\n"
"color: rgb(52, 101, 164);"));
        X_relative_PH_dup->setReadOnly(false);

        verticalLayout_10->addWidget(X_relative_PH_dup);

        label_84 = new QLabel(verticalLayoutWidget_7);
        label_84->setObjectName(QString::fromUtf8("label_84"));
        label_84->setFont(font);
        label_84->setAlignment(Qt::AlignCenter);

        verticalLayout_10->addWidget(label_84);

        Y_relative_PH_dup = new QTextEdit(verticalLayoutWidget_7);
        Y_relative_PH_dup->setObjectName(QString::fromUtf8("Y_relative_PH_dup"));
        Y_relative_PH_dup->setEnabled(true);
        Y_relative_PH_dup->setStyleSheet(QString::fromUtf8("font: 28px;\n"
"color: rgb(52, 101, 164);"));
        Y_relative_PH_dup->setReadOnly(false);

        verticalLayout_10->addWidget(Y_relative_PH_dup);

        label_85 = new QLabel(verticalLayoutWidget_7);
        label_85->setObjectName(QString::fromUtf8("label_85"));
        label_85->setFont(font);
        label_85->setAlignment(Qt::AlignCenter);

        verticalLayout_10->addWidget(label_85);

        Z_relative_PH_dup = new QTextEdit(verticalLayoutWidget_7);
        Z_relative_PH_dup->setObjectName(QString::fromUtf8("Z_relative_PH_dup"));
        Z_relative_PH_dup->setEnabled(true);
        Z_relative_PH_dup->setStyleSheet(QString::fromUtf8("font: 28px;\n"
"color: rgb(52, 101, 164);"));
        Z_relative_PH_dup->setReadOnly(false);

        verticalLayout_10->addWidget(Z_relative_PH_dup);

        horizontalLayoutWidget_3 = new QWidget(axis);
        horizontalLayoutWidget_3->setObjectName(QString::fromUtf8("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(80, 380, 741, 72));
        horizontalLayout_5 = new QHBoxLayout(horizontalLayoutWidget_3);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        label_43 = new QLabel(horizontalLayoutWidget_3);
        label_43->setObjectName(QString::fromUtf8("label_43"));

        horizontalLayout_5->addWidget(label_43);

        input_thickness_dup = new QTextEdit(horizontalLayoutWidget_3);
        input_thickness_dup->setObjectName(QString::fromUtf8("input_thickness_dup"));
        input_thickness_dup->setEnabled(true);
        input_thickness_dup->setStyleSheet(QString::fromUtf8("font: 28px;\n"
"color: rgb(52, 101, 164);"));
        input_thickness_dup->setTabChangesFocus(false);
        input_thickness_dup->setReadOnly(true);

        horizontalLayout_5->addWidget(input_thickness_dup);

        label_42 = new QLabel(horizontalLayoutWidget_3);
        label_42->setObjectName(QString::fromUtf8("label_42"));

        horizontalLayout_5->addWidget(label_42);

        input_gap_dup = new QTextEdit(horizontalLayoutWidget_3);
        input_gap_dup->setObjectName(QString::fromUtf8("input_gap_dup"));
        input_gap_dup->setEnabled(true);
        input_gap_dup->setStyleSheet(QString::fromUtf8("font: 28px;\n"
"color: rgb(52, 101, 164);"));
        input_gap_dup->setReadOnly(true);

        horizontalLayout_5->addWidget(input_gap_dup);

        label_38 = new QLabel(horizontalLayoutWidget_3);
        label_38->setObjectName(QString::fromUtf8("label_38"));

        horizontalLayout_5->addWidget(label_38);

        input_overlap_dup = new QTextEdit(horizontalLayoutWidget_3);
        input_overlap_dup->setObjectName(QString::fromUtf8("input_overlap_dup"));
        input_overlap_dup->setEnabled(true);
        input_overlap_dup->setStyleSheet(QString::fromUtf8("font: 28px;\n"
"color: rgb(52, 101, 164);"));
        input_overlap_dup->setReadOnly(true);

        horizontalLayout_5->addWidget(input_overlap_dup);

        label_40 = new QLabel(horizontalLayoutWidget_3);
        label_40->setObjectName(QString::fromUtf8("label_40"));

        horizontalLayout_5->addWidget(label_40);

        input_speed_dup = new QTextEdit(horizontalLayoutWidget_3);
        input_speed_dup->setObjectName(QString::fromUtf8("input_speed_dup"));
        input_speed_dup->setEnabled(true);
        input_speed_dup->setStyleSheet(QString::fromUtf8("font: 28px;\n"
"color: rgb(52, 101, 164);"));
        input_speed_dup->setReadOnly(true);

        horizontalLayout_5->addWidget(input_speed_dup);

        label_41 = new QLabel(horizontalLayoutWidget_3);
        label_41->setObjectName(QString::fromUtf8("label_41"));

        horizontalLayout_5->addWidget(label_41);

        input_cycles_dup = new QTextEdit(horizontalLayoutWidget_3);
        input_cycles_dup->setObjectName(QString::fromUtf8("input_cycles_dup"));
        input_cycles_dup->setEnabled(true);
        input_cycles_dup->setStyleSheet(QString::fromUtf8("font: 28px;\n"
"color: rgb(52, 101, 164);"));
        input_cycles_dup->setReadOnly(true);

        horizontalLayout_5->addWidget(input_cycles_dup);

        gridLayoutWidget_2 = new QWidget(axis);
        gridLayoutWidget_2->setObjectName(QString::fromUtf8("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(770, 490, 310, 171));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        xmin_controls_dup = new QTextEdit(gridLayoutWidget_2);
        xmin_controls_dup->setObjectName(QString::fromUtf8("xmin_controls_dup"));
        xmin_controls_dup->setEnabled(true);
        xmin_controls_dup->setReadOnly(true);

        gridLayout_2->addWidget(xmin_controls_dup, 0, 2, 1, 1);

        ymax_controls_dup = new QTextEdit(gridLayoutWidget_2);
        ymax_controls_dup->setObjectName(QString::fromUtf8("ymax_controls_dup"));
        ymax_controls_dup->setEnabled(true);
        ymax_controls_dup->setReadOnly(true);

        gridLayout_2->addWidget(ymax_controls_dup, 1, 4, 1, 1);

        label_47 = new QLabel(gridLayoutWidget_2);
        label_47->setObjectName(QString::fromUtf8("label_47"));
        sizePolicy.setHeightForWidth(label_47->sizePolicy().hasHeightForWidth());
        label_47->setSizePolicy(sizePolicy);
        label_47->setMinimumSize(QSize(75, 75));
        label_47->setFont(font);
        label_47->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_47, 1, 1, 1, 1);

        ymin_controls_dup = new QTextEdit(gridLayoutWidget_2);
        ymin_controls_dup->setObjectName(QString::fromUtf8("ymin_controls_dup"));
        ymin_controls_dup->setEnabled(true);
        ymin_controls_dup->setReadOnly(true);

        gridLayout_2->addWidget(ymin_controls_dup, 1, 2, 1, 1);

        xmax_controls_dup = new QTextEdit(gridLayoutWidget_2);
        xmax_controls_dup->setObjectName(QString::fromUtf8("xmax_controls_dup"));
        xmax_controls_dup->setEnabled(true);
        xmax_controls_dup->setReadOnly(true);

        gridLayout_2->addWidget(xmax_controls_dup, 0, 4, 1, 1);

        label_35 = new QLabel(gridLayoutWidget_2);
        label_35->setObjectName(QString::fromUtf8("label_35"));
        sizePolicy.setHeightForWidth(label_35->sizePolicy().hasHeightForWidth());
        label_35->setSizePolicy(sizePolicy);
        label_35->setMinimumSize(QSize(75, 75));
        label_35->setFont(font);
        label_35->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_35, 0, 1, 1, 1);

        label_44 = new QLabel(gridLayoutWidget_2);
        label_44->setObjectName(QString::fromUtf8("label_44"));
        sizePolicy.setHeightForWidth(label_44->sizePolicy().hasHeightForWidth());
        label_44->setSizePolicy(sizePolicy);
        label_44->setMinimumSize(QSize(75, 75));
        label_44->setFont(font);
        label_44->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_44, 0, 3, 1, 1);

        label_48 = new QLabel(gridLayoutWidget_2);
        label_48->setObjectName(QString::fromUtf8("label_48"));
        sizePolicy.setHeightForWidth(label_48->sizePolicy().hasHeightForWidth());
        label_48->setSizePolicy(sizePolicy);
        label_48->setMinimumSize(QSize(75, 75));
        label_48->setFont(font);
        label_48->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_48, 1, 3, 1, 1);

        label_46 = new QLabel(axis);
        label_46->setObjectName(QString::fromUtf8("label_46"));
        label_46->setGeometry(QRect(780, 470, 161, 16));
        label_37 = new QLabel(axis);
        label_37->setObjectName(QString::fromUtf8("label_37"));
        label_37->setGeometry(QRect(950, 40, 119, 16));
        gridLayoutWidget_5 = new QWidget(axis);
        gridLayoutWidget_5->setObjectName(QString::fromUtf8("gridLayoutWidget_5"));
        gridLayoutWidget_5->setGeometry(QRect(60, 473, 681, 196));
        gridLayout_5 = new QGridLayout(gridLayoutWidget_5);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        label_24 = new QLabel(gridLayoutWidget_5);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        verticalLayout_12->addWidget(label_24);

        axisstatus_dup = new QTextEdit(gridLayoutWidget_5);
        axisstatus_dup->setObjectName(QString::fromUtf8("axisstatus_dup"));
        axisstatus_dup->setEnabled(true);
        axisstatus_dup->setReadOnly(true);

        verticalLayout_12->addWidget(axisstatus_dup);

        label_25 = new QLabel(gridLayoutWidget_5);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        verticalLayout_12->addWidget(label_25);

        axisstatus_2_dup = new QTextEdit(gridLayoutWidget_5);
        axisstatus_2_dup->setObjectName(QString::fromUtf8("axisstatus_2_dup"));
        axisstatus_2_dup->setEnabled(true);
        axisstatus_2_dup->setReadOnly(true);

        verticalLayout_12->addWidget(axisstatus_2_dup);


        gridLayout_5->addLayout(verticalLayout_12, 0, 1, 1, 1);

        door2 = new QPushButton(gridLayoutWidget_5);
        door2->setObjectName(QString::fromUtf8("door2"));
        sizePolicy22.setHeightForWidth(door2->sizePolicy().hasHeightForWidth());
        door2->setSizePolicy(sizePolicy22);
        door2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_5->addWidget(door2, 0, 2, 1, 1);

        door1 = new QPushButton(gridLayoutWidget_5);
        door1->setObjectName(QString::fromUtf8("door1"));
        door1->setEnabled(true);
        sizePolicy22.setHeightForWidth(door1->sizePolicy().hasHeightForWidth());
        door1->setSizePolicy(sizePolicy22);
        door1->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_5->addWidget(door1, 0, 0, 1, 1);

        QIcon icon9;
        iconThemeName = QString::fromUtf8("applications-development");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon9 = QIcon::fromTheme(iconThemeName);
        } else {
            icon9.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        tabWidget->addTab(axis, icon9, QString());
        settings = new QWidget();
        settings->setObjectName(QString::fromUtf8("settings"));
        groupBox_2 = new QGroupBox(settings);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(40, 30, 1181, 631));
        gridLayoutWidget_4 = new QWidget(groupBox_2);
        gridLayoutWidget_4->setObjectName(QString::fromUtf8("gridLayoutWidget_4"));
        gridLayoutWidget_4->setGeometry(QRect(60, 80, 671, 271));
        gridLayout_4 = new QGridLayout(gridLayoutWidget_4);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        heater_checkbox = new QCheckBox(gridLayoutWidget_4);
        heater_checkbox->setObjectName(QString::fromUtf8("heater_checkbox"));
        heater_checkbox->setLayoutDirection(Qt::LeftToRight);
        QIcon icon10;
        iconThemeName = QString::fromUtf8("weather-clear");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon10 = QIcon::fromTheme(iconThemeName);
        } else {
            icon10.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        heater_checkbox->setIcon(icon10);
        heater_checkbox->setIconSize(QSize(32, 32));

        gridLayout_4->addWidget(heater_checkbox, 1, 0, 1, 1);

        ctldiagnostics_checkbox = new QCheckBox(gridLayoutWidget_4);
        ctldiagnostics_checkbox->setObjectName(QString::fromUtf8("ctldiagnostics_checkbox"));
        QIcon icon11;
        iconThemeName = QString::fromUtf8("emblem-system");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon11 = QIcon::fromTheme(iconThemeName);
        } else {
            icon11.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        ctldiagnostics_checkbox->setIcon(icon11);
        ctldiagnostics_checkbox->setIconSize(QSize(32, 32));

        gridLayout_4->addWidget(ctldiagnostics_checkbox, 3, 0, 1, 1);

        mfc_loaded_checkbox = new QCheckBox(gridLayoutWidget_4);
        mfc_loaded_checkbox->setObjectName(QString::fromUtf8("mfc_loaded_checkbox"));
        QIcon icon12;
        iconThemeName = QString::fromUtf8("help-about");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon12 = QIcon::fromTheme(iconThemeName);
        } else {
            icon12.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        mfc_loaded_checkbox->setIcon(icon12);
        mfc_loaded_checkbox->setIconSize(QSize(32, 32));

        gridLayout_4->addWidget(mfc_loaded_checkbox, 4, 0, 1, 1);

        batchID_checkBox = new QCheckBox(gridLayoutWidget_4);
        batchID_checkBox->setObjectName(QString::fromUtf8("batchID_checkBox"));
        QIcon icon13;
        iconThemeName = QString::fromUtf8("emblem-documents");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon13 = QIcon::fromTheme(iconThemeName);
        } else {
            icon13.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        batchID_checkBox->setIcon(icon13);
        batchID_checkBox->setIconSize(QSize(32, 32));

        gridLayout_4->addWidget(batchID_checkBox, 2, 0, 1, 1);

        request_terminal = new QCheckBox(gridLayoutWidget_4);
        request_terminal->setObjectName(QString::fromUtf8("request_terminal"));
        QIcon icon14;
        iconThemeName = QString::fromUtf8("utilities-terminal");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon14 = QIcon::fromTheme(iconThemeName);
        } else {
            icon14.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        request_terminal->setIcon(icon14);
        request_terminal->setIconSize(QSize(32, 32));

        gridLayout_4->addWidget(request_terminal, 0, 0, 1, 1);

        QIcon icon15;
        iconThemeName = QString::fromUtf8("preferences-system");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon15 = QIcon::fromTheme(iconThemeName);
        } else {
            icon15.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        tabWidget->addTab(settings, icon15, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        QIcon icon16;
        iconThemeName = QString::fromUtf8("system-lock-screen");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon16 = QIcon::fromTheme(iconThemeName);
        } else {
            icon16.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        tabWidget->addTab(tab, icon16, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        groupBox_5 = new QGroupBox(tab_2);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setGeometry(QRect(70, 40, 1291, 661));
        gridLayoutWidget_15 = new QWidget(groupBox_5);
        gridLayoutWidget_15->setObjectName(QString::fromUtf8("gridLayoutWidget_15"));
        gridLayoutWidget_15->setGeometry(QRect(30, 40, 1215, 634));
        gridLayout_31 = new QGridLayout(gridLayoutWidget_15);
        gridLayout_31->setObjectName(QString::fromUtf8("gridLayout_31"));
        gridLayout_31->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_38 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_31->addItem(horizontalSpacer_38, 3, 2, 1, 1);

        gridLayout_32 = new QGridLayout();
        gridLayout_32->setObjectName(QString::fromUtf8("gridLayout_32"));
        label_115 = new QLabel(gridLayoutWidget_15);
        label_115->setObjectName(QString::fromUtf8("label_115"));
        label_115->setStyleSheet(QString::fromUtf8("\n"
"font: 700 20pt \"Ubuntu Mono\";"));

        gridLayout_32->addWidget(label_115, 0, 0, 1, 2);

        autotune_recipe_dup_3 = new QPushButton(gridLayoutWidget_15);
        autotune_recipe_dup_3->setObjectName(QString::fromUtf8("autotune_recipe_dup_3"));
        autotune_recipe_dup_3->setMinimumSize(QSize(0, 50));
        autotune_recipe_dup_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"font: 16pt \"Ubuntu Bold\";\n"
"	color: rgb(220, 0, 20);\n"
"}"));
        autotune_recipe_dup_3->setIconSize(QSize(32, 32));

        gridLayout_32->addWidget(autotune_recipe_dup_3, 4, 1, 1, 1);

        load_RF_dup_3 = new QPushButton(gridLayoutWidget_15);
        load_RF_dup_3->setObjectName(QString::fromUtf8("load_RF_dup_3"));
        sizePolicy22.setHeightForWidth(load_RF_dup_3->sizePolicy().hasHeightForWidth());
        load_RF_dup_3->setSizePolicy(sizePolicy22);
        load_RF_dup_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_32->addWidget(load_RF_dup_3, 1, 0, 1, 1);

        load_MB_dup_3 = new QPushButton(gridLayoutWidget_15);
        load_MB_dup_3->setObjectName(QString::fromUtf8("load_MB_dup_3"));
        sizePolicy22.setHeightForWidth(load_MB_dup_3->sizePolicy().hasHeightForWidth());
        load_MB_dup_3->setSizePolicy(sizePolicy22);
        load_MB_dup_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_32->addWidget(load_MB_dup_3, 3, 0, 1, 1);

        mb_recipe_dup_3 = new QTextEdit(gridLayoutWidget_15);
        mb_recipe_dup_3->setObjectName(QString::fromUtf8("mb_recipe_dup_3"));
        mb_recipe_dup_3->setStyleSheet(QString::fromUtf8("font: 18px;\n"
"color: rgb(52, 101, 164);"));
        mb_recipe_dup_3->setReadOnly(true);

        gridLayout_32->addWidget(mb_recipe_dup_3, 3, 1, 1, 1);

        RF_recipe_dup_3 = new QTextEdit(gridLayoutWidget_15);
        RF_recipe_dup_3->setObjectName(QString::fromUtf8("RF_recipe_dup_3"));
        RF_recipe_dup_3->setStyleSheet(QString::fromUtf8("font: 18px;\n"
"color: rgb(52, 101, 164);"));
        RF_recipe_dup_3->setReadOnly(true);

        gridLayout_32->addWidget(RF_recipe_dup_3, 1, 1, 1, 1);

        label_116 = new QLabel(gridLayoutWidget_15);
        label_116->setObjectName(QString::fromUtf8("label_116"));
        label_116->setStyleSheet(QString::fromUtf8("\n"
"font: 700 20pt \"Ubuntu Mono\";"));

        gridLayout_32->addWidget(label_116, 2, 0, 1, 2);

        load_autotune_dup_3 = new QPushButton(gridLayoutWidget_15);
        load_autotune_dup_3->setObjectName(QString::fromUtf8("load_autotune_dup_3"));
        sizePolicy22.setHeightForWidth(load_autotune_dup_3->sizePolicy().hasHeightForWidth());
        load_autotune_dup_3->setSizePolicy(sizePolicy22);
        load_autotune_dup_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_32->addWidget(load_autotune_dup_3, 4, 0, 1, 1);


        gridLayout_31->addLayout(gridLayout_32, 3, 1, 1, 1);

        horizontalSpacer_39 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_31->addItem(horizontalSpacer_39, 4, 0, 1, 4);

        gridLayout_33 = new QGridLayout();
        gridLayout_33->setObjectName(QString::fromUtf8("gridLayout_33"));
        gridLayout_34 = new QGridLayout();
        gridLayout_34->setObjectName(QString::fromUtf8("gridLayout_34"));
        horizontalSpacer_40 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_34->addItem(horizontalSpacer_40, 6, 0, 1, 1);

        horizontalSpacer_41 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_34->addItem(horizontalSpacer_41, 4, 1, 1, 1);

        horizontalSpacer_42 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_34->addItem(horizontalSpacer_42, 10, 0, 1, 1);

        horizontalSpacer_43 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_34->addItem(horizontalSpacer_43, 7, 1, 1, 1);

        horizontalSpacer_44 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_34->addItem(horizontalSpacer_44, 5, 0, 1, 1);

        horizontalSpacer_45 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_34->addItem(horizontalSpacer_45, 7, 0, 1, 1);

        horizontalSpacer_46 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_34->addItem(horizontalSpacer_46, 8, 0, 1, 1);

        label_117 = new QLabel(gridLayoutWidget_15);
        label_117->setObjectName(QString::fromUtf8("label_117"));
        label_117->setStyleSheet(QString::fromUtf8("\n"
"font: 700 20pt \"Ubuntu Mono\";"));

        gridLayout_34->addWidget(label_117, 0, 0, 1, 2);

        horizontalSpacer_47 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_34->addItem(horizontalSpacer_47, 5, 1, 1, 1);

        horizontalSpacer_48 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_34->addItem(horizontalSpacer_48, 9, 1, 1, 1);

        horizontalSpacer_49 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_34->addItem(horizontalSpacer_49, 10, 1, 1, 1);

        cycles_recipe_dup_3 = new QTextEdit(gridLayoutWidget_15);
        cycles_recipe_dup_3->setObjectName(QString::fromUtf8("cycles_recipe_dup_3"));
        cycles_recipe_dup_3->setStyleSheet(QString::fromUtf8("font: 18px;\n"
"color: rgb(52, 101, 164);"));
        cycles_recipe_dup_3->setReadOnly(true);

        gridLayout_34->addWidget(cycles_recipe_dup_3, 2, 1, 1, 1);

        horizontalSpacer_50 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_34->addItem(horizontalSpacer_50, 6, 1, 1, 1);

        horizontalSpacer_51 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_34->addItem(horizontalSpacer_51, 8, 1, 1, 1);

        horizontalSpacer_52 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_34->addItem(horizontalSpacer_52, 4, 0, 1, 1);

        speed_recipe_dup_3 = new QTextEdit(gridLayoutWidget_15);
        speed_recipe_dup_3->setObjectName(QString::fromUtf8("speed_recipe_dup_3"));
        speed_recipe_dup_3->setStyleSheet(QString::fromUtf8("font: 18px;\n"
"color: rgb(52, 101, 164);"));
        speed_recipe_dup_3->setReadOnly(true);

        gridLayout_34->addWidget(speed_recipe_dup_3, 1, 1, 1, 1);

        autoscan_recipe_dup_3 = new QPushButton(gridLayoutWidget_15);
        autoscan_recipe_dup_3->setObjectName(QString::fromUtf8("autoscan_recipe_dup_3"));
        autoscan_recipe_dup_3->setMinimumSize(QSize(0, 50));
        autoscan_recipe_dup_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"font: 16pt \"Ubuntu Bold\";\n"
"	color: rgb(220, 0, 20);\n"
"}"));
        autoscan_recipe_dup_3->setIconSize(QSize(32, 32));

        gridLayout_34->addWidget(autoscan_recipe_dup_3, 3, 1, 1, 1);

        horizontalSpacer_53 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_34->addItem(horizontalSpacer_53, 9, 0, 1, 1);

        load_autoscan_dup_3 = new QPushButton(gridLayoutWidget_15);
        load_autoscan_dup_3->setObjectName(QString::fromUtf8("load_autoscan_dup_3"));
        sizePolicy22.setHeightForWidth(load_autoscan_dup_3->sizePolicy().hasHeightForWidth());
        load_autoscan_dup_3->setSizePolicy(sizePolicy22);
        load_autoscan_dup_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_34->addWidget(load_autoscan_dup_3, 3, 0, 1, 1);

        load_cycles_dup_3 = new QPushButton(gridLayoutWidget_15);
        load_cycles_dup_3->setObjectName(QString::fromUtf8("load_cycles_dup_3"));
        sizePolicy22.setHeightForWidth(load_cycles_dup_3->sizePolicy().hasHeightForWidth());
        load_cycles_dup_3->setSizePolicy(sizePolicy22);
        load_cycles_dup_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_34->addWidget(load_cycles_dup_3, 2, 0, 1, 1);

        load_speed_dup_3 = new QPushButton(gridLayoutWidget_15);
        load_speed_dup_3->setObjectName(QString::fromUtf8("load_speed_dup_3"));
        sizePolicy22.setHeightForWidth(load_speed_dup_3->sizePolicy().hasHeightForWidth());
        load_speed_dup_3->setSizePolicy(sizePolicy22);
        load_speed_dup_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_34->addWidget(load_speed_dup_3, 1, 0, 1, 1);


        gridLayout_33->addLayout(gridLayout_34, 0, 1, 1, 1);

        gridLayout_35 = new QGridLayout();
        gridLayout_35->setObjectName(QString::fromUtf8("gridLayout_35"));
        ymin_recipe_dup_3 = new QTextEdit(gridLayoutWidget_15);
        ymin_recipe_dup_3->setObjectName(QString::fromUtf8("ymin_recipe_dup_3"));
        ymin_recipe_dup_3->setStyleSheet(QString::fromUtf8("font: 18px;\n"
"color: rgb(52, 101, 164);"));
        ymin_recipe_dup_3->setReadOnly(true);

        gridLayout_35->addWidget(ymin_recipe_dup_3, 6, 1, 1, 1);

        load_thick_dup_3 = new QPushButton(gridLayoutWidget_15);
        load_thick_dup_3->setObjectName(QString::fromUtf8("load_thick_dup_3"));
        sizePolicy22.setHeightForWidth(load_thick_dup_3->sizePolicy().hasHeightForWidth());
        load_thick_dup_3->setSizePolicy(sizePolicy22);
        load_thick_dup_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_35->addWidget(load_thick_dup_3, 1, 0, 1, 2);

        xmax_recipe_dup_3 = new QTextEdit(gridLayoutWidget_15);
        xmax_recipe_dup_3->setObjectName(QString::fromUtf8("xmax_recipe_dup_3"));
        xmax_recipe_dup_3->setStyleSheet(QString::fromUtf8("font: 18px;\n"
"color: rgb(52, 101, 164);"));
        xmax_recipe_dup_3->setReadOnly(true);

        gridLayout_35->addWidget(xmax_recipe_dup_3, 5, 3, 1, 1);

        label_118 = new QLabel(gridLayoutWidget_15);
        label_118->setObjectName(QString::fromUtf8("label_118"));
        label_118->setStyleSheet(QString::fromUtf8("\n"
"font: 700 20pt \"Ubuntu Mono\";"));

        gridLayout_35->addWidget(label_118, 0, 0, 1, 4);

        x1_set_dup_3 = new QPushButton(gridLayoutWidget_15);
        x1_set_dup_3->setObjectName(QString::fromUtf8("x1_set_dup_3"));
        sizePolicy22.setHeightForWidth(x1_set_dup_3->sizePolicy().hasHeightForWidth());
        x1_set_dup_3->setSizePolicy(sizePolicy22);
        x1_set_dup_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_35->addWidget(x1_set_dup_3, 5, 0, 1, 1);

        xmin_recipe_dup_3 = new QTextEdit(gridLayoutWidget_15);
        xmin_recipe_dup_3->setObjectName(QString::fromUtf8("xmin_recipe_dup_3"));
        xmin_recipe_dup_3->setStyleSheet(QString::fromUtf8("font: 18px;\n"
"color: rgb(52, 101, 164);"));
        xmin_recipe_dup_3->setReadOnly(true);

        gridLayout_35->addWidget(xmin_recipe_dup_3, 5, 1, 1, 1);

        ymax_recipe_dup_3 = new QTextEdit(gridLayoutWidget_15);
        ymax_recipe_dup_3->setObjectName(QString::fromUtf8("ymax_recipe_dup_3"));
        ymax_recipe_dup_3->setStyleSheet(QString::fromUtf8("font: 18px;\n"
"color: rgb(52, 101, 164);"));
        ymax_recipe_dup_3->setReadOnly(true);

        gridLayout_35->addWidget(ymax_recipe_dup_3, 6, 3, 1, 1);

        gap_recipe_dup_3 = new QTextEdit(gridLayoutWidget_15);
        gap_recipe_dup_3->setObjectName(QString::fromUtf8("gap_recipe_dup_3"));
        gap_recipe_dup_3->setStyleSheet(QString::fromUtf8("font: 18px;\n"
"color: rgb(52, 101, 164);"));
        gap_recipe_dup_3->setReadOnly(true);

        gridLayout_35->addWidget(gap_recipe_dup_3, 2, 2, 1, 2);

        overlap_recipe_dup_3 = new QTextEdit(gridLayoutWidget_15);
        overlap_recipe_dup_3->setObjectName(QString::fromUtf8("overlap_recipe_dup_3"));
        overlap_recipe_dup_3->setStyleSheet(QString::fromUtf8("font: 18px;\n"
"color: rgb(52, 101, 164);"));
        overlap_recipe_dup_3->setReadOnly(true);

        gridLayout_35->addWidget(overlap_recipe_dup_3, 3, 2, 1, 2);

        Y1_set_dup_3 = new QPushButton(gridLayoutWidget_15);
        Y1_set_dup_3->setObjectName(QString::fromUtf8("Y1_set_dup_3"));
        sizePolicy22.setHeightForWidth(Y1_set_dup_3->sizePolicy().hasHeightForWidth());
        Y1_set_dup_3->setSizePolicy(sizePolicy22);
        Y1_set_dup_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_35->addWidget(Y1_set_dup_3, 6, 0, 1, 1);

        thickness_recipe_dup_3 = new QTextEdit(gridLayoutWidget_15);
        thickness_recipe_dup_3->setObjectName(QString::fromUtf8("thickness_recipe_dup_3"));
        thickness_recipe_dup_3->setStyleSheet(QString::fromUtf8("font: 18px;\n"
"color: rgb(52, 101, 164);"));
        thickness_recipe_dup_3->setReadOnly(true);

        gridLayout_35->addWidget(thickness_recipe_dup_3, 1, 2, 1, 2);

        load_overlap_dup_3 = new QPushButton(gridLayoutWidget_15);
        load_overlap_dup_3->setObjectName(QString::fromUtf8("load_overlap_dup_3"));
        sizePolicy22.setHeightForWidth(load_overlap_dup_3->sizePolicy().hasHeightForWidth());
        load_overlap_dup_3->setSizePolicy(sizePolicy22);
        load_overlap_dup_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_35->addWidget(load_overlap_dup_3, 3, 0, 1, 2);

        load_gap_dup_3 = new QPushButton(gridLayoutWidget_15);
        load_gap_dup_3->setObjectName(QString::fromUtf8("load_gap_dup_3"));
        sizePolicy22.setHeightForWidth(load_gap_dup_3->sizePolicy().hasHeightForWidth());
        load_gap_dup_3->setSizePolicy(sizePolicy22);
        load_gap_dup_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_35->addWidget(load_gap_dup_3, 2, 0, 1, 2);

        x2_set_dup_3 = new QPushButton(gridLayoutWidget_15);
        x2_set_dup_3->setObjectName(QString::fromUtf8("x2_set_dup_3"));
        sizePolicy22.setHeightForWidth(x2_set_dup_3->sizePolicy().hasHeightForWidth());
        x2_set_dup_3->setSizePolicy(sizePolicy22);
        x2_set_dup_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_35->addWidget(x2_set_dup_3, 5, 2, 1, 1);

        Y2_set_dup_3 = new QPushButton(gridLayoutWidget_15);
        Y2_set_dup_3->setObjectName(QString::fromUtf8("Y2_set_dup_3"));
        sizePolicy22.setHeightForWidth(Y2_set_dup_3->sizePolicy().hasHeightForWidth());
        Y2_set_dup_3->setSizePolicy(sizePolicy22);
        Y2_set_dup_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_35->addWidget(Y2_set_dup_3, 6, 2, 1, 1);

        label_119 = new QLabel(gridLayoutWidget_15);
        label_119->setObjectName(QString::fromUtf8("label_119"));
        label_119->setStyleSheet(QString::fromUtf8("\n"
"font: 700 20pt \"Ubuntu Mono\";"));

        gridLayout_35->addWidget(label_119, 4, 0, 1, 4);


        gridLayout_33->addLayout(gridLayout_35, 0, 0, 1, 1);


        gridLayout_31->addLayout(gridLayout_33, 3, 3, 1, 1);

        label_120 = new QLabel(gridLayoutWidget_15);
        label_120->setObjectName(QString::fromUtf8("label_120"));
        label_120->setStyleSheet(QString::fromUtf8("font: 16px;"));

        gridLayout_31->addWidget(label_120, 2, 3, 1, 1);

        label_121 = new QLabel(gridLayoutWidget_15);
        label_121->setObjectName(QString::fromUtf8("label_121"));
        label_121->setStyleSheet(QString::fromUtf8("font: 16px;"));

        gridLayout_31->addWidget(label_121, 0, 0, 1, 2);

        name_recipe_dup_3 = new QTextEdit(gridLayoutWidget_15);
        name_recipe_dup_3->setObjectName(QString::fromUtf8("name_recipe_dup_3"));
        sizePolicy14.setHeightForWidth(name_recipe_dup_3->sizePolicy().hasHeightForWidth());
        name_recipe_dup_3->setSizePolicy(sizePolicy14);
        name_recipe_dup_3->setMaximumSize(QSize(16777215, 35));
        name_recipe_dup_3->setStyleSheet(QString::fromUtf8("font: 16px;"));

        gridLayout_31->addWidget(name_recipe_dup_3, 1, 0, 1, 2);

        label_122 = new QLabel(gridLayoutWidget_15);
        label_122->setObjectName(QString::fromUtf8("label_122"));
        label_122->setStyleSheet(QString::fromUtf8("font: 16px;"));

        gridLayout_31->addWidget(label_122, 2, 0, 1, 2);

        save_recipe_button_dup_3 = new QPushButton(gridLayoutWidget_15);
        save_recipe_button_dup_3->setObjectName(QString::fromUtf8("save_recipe_button_dup_3"));
        save_recipe_button_dup_3->setMinimumSize(QSize(0, 50));
        save_recipe_button_dup_3->setStyleSheet(QString::fromUtf8("\n"
"QPushButton {\n"
"color: white;\n"
"background-color: green;\n"
"font: 14pt \"Ubuntu Bold\";\n"
"}"));
        save_recipe_button_dup_3->setIcon(icon4);
        save_recipe_button_dup_3->setIconSize(QSize(32, 32));

        gridLayout_31->addWidget(save_recipe_button_dup_3, 5, 0, 1, 4);

        load_recipe_button_dup_3 = new QPushButton(gridLayoutWidget_15);
        load_recipe_button_dup_3->setObjectName(QString::fromUtf8("load_recipe_button_dup_3"));
        load_recipe_button_dup_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"color: white;\n"
"background-color: rgb(119, 136, 153);\n"
"font: 14pt \"Ubuntu Bold\";\n"
"}\n"
""));
        QIcon icon17;
        iconThemeName = QString::fromUtf8("accessories-text-editor");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon17 = QIcon::fromTheme(iconThemeName);
        } else {
            icon17.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        load_recipe_button_dup_3->setIcon(icon17);
        load_recipe_button_dup_3->setIconSize(QSize(32, 32));

        gridLayout_31->addWidget(load_recipe_button_dup_3, 1, 2, 1, 1);

        gridLayout_36 = new QGridLayout();
        gridLayout_36->setObjectName(QString::fromUtf8("gridLayout_36"));
        mfc1_recipe_dup_3 = new QTextEdit(gridLayoutWidget_15);
        mfc1_recipe_dup_3->setObjectName(QString::fromUtf8("mfc1_recipe_dup_3"));
        mfc1_recipe_dup_3->setEnabled(true);
        mfc1_recipe_dup_3->setStyleSheet(QString::fromUtf8("font: 28px;"));
        mfc1_recipe_dup_3->setReadOnly(true);

        gridLayout_36->addWidget(mfc1_recipe_dup_3, 1, 1, 1, 1);

        mfc4_recipe_dup_3 = new QTextEdit(gridLayoutWidget_15);
        mfc4_recipe_dup_3->setObjectName(QString::fromUtf8("mfc4_recipe_dup_3"));
        mfc4_recipe_dup_3->setEnabled(true);
        mfc4_recipe_dup_3->setStyleSheet(QString::fromUtf8("font: 28px;"));
        mfc4_recipe_dup_3->setReadOnly(true);

        gridLayout_36->addWidget(mfc4_recipe_dup_3, 4, 1, 1, 1);

        mfc2_recipe_dup_3 = new QTextEdit(gridLayoutWidget_15);
        mfc2_recipe_dup_3->setObjectName(QString::fromUtf8("mfc2_recipe_dup_3"));
        mfc2_recipe_dup_3->setEnabled(true);
        mfc2_recipe_dup_3->setStyleSheet(QString::fromUtf8("font: 28px;"));
        mfc2_recipe_dup_3->setReadOnly(true);

        gridLayout_36->addWidget(mfc2_recipe_dup_3, 2, 1, 1, 1);

        mfc3_recipe_dup_3 = new QTextEdit(gridLayoutWidget_15);
        mfc3_recipe_dup_3->setObjectName(QString::fromUtf8("mfc3_recipe_dup_3"));
        mfc3_recipe_dup_3->setEnabled(true);
        mfc3_recipe_dup_3->setStyleSheet(QString::fromUtf8("font: 28px;"));
        mfc3_recipe_dup_3->setReadOnly(true);

        gridLayout_36->addWidget(mfc3_recipe_dup_3, 3, 1, 1, 1);

        label_123 = new QLabel(gridLayoutWidget_15);
        label_123->setObjectName(QString::fromUtf8("label_123"));
        label_123->setStyleSheet(QString::fromUtf8("\n"
"font: 700 20pt \"Ubuntu Mono\";"));

        gridLayout_36->addWidget(label_123, 0, 0, 1, 2);

        load_MFC1_dup_3 = new QPushButton(gridLayoutWidget_15);
        load_MFC1_dup_3->setObjectName(QString::fromUtf8("load_MFC1_dup_3"));
        sizePolicy22.setHeightForWidth(load_MFC1_dup_3->sizePolicy().hasHeightForWidth());
        load_MFC1_dup_3->setSizePolicy(sizePolicy22);
        load_MFC1_dup_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_36->addWidget(load_MFC1_dup_3, 1, 0, 1, 1);

        load_MFC2_dup_3 = new QPushButton(gridLayoutWidget_15);
        load_MFC2_dup_3->setObjectName(QString::fromUtf8("load_MFC2_dup_3"));
        sizePolicy22.setHeightForWidth(load_MFC2_dup_3->sizePolicy().hasHeightForWidth());
        load_MFC2_dup_3->setSizePolicy(sizePolicy22);
        load_MFC2_dup_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_36->addWidget(load_MFC2_dup_3, 2, 0, 1, 1);

        load_MFC4_dup_3 = new QPushButton(gridLayoutWidget_15);
        load_MFC4_dup_3->setObjectName(QString::fromUtf8("load_MFC4_dup_3"));
        sizePolicy22.setHeightForWidth(load_MFC4_dup_3->sizePolicy().hasHeightForWidth());
        load_MFC4_dup_3->setSizePolicy(sizePolicy22);
        load_MFC4_dup_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_36->addWidget(load_MFC4_dup_3, 4, 0, 1, 1);

        load_MFC3_dup_3 = new QPushButton(gridLayoutWidget_15);
        load_MFC3_dup_3->setObjectName(QString::fromUtf8("load_MFC3_dup_3"));
        sizePolicy22.setHeightForWidth(load_MFC3_dup_3->sizePolicy().hasHeightForWidth());
        load_MFC3_dup_3->setSizePolicy(sizePolicy22);
        load_MFC3_dup_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(119, 136, 153);\n"
"font: 18pt \"Ubuntu Bold\";\n"
"}"));

        gridLayout_36->addWidget(load_MFC3_dup_3, 3, 0, 1, 1);


        gridLayout_31->addLayout(gridLayout_36, 3, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());
        MainWindow->setCentralWidget(centralwidget);
        toolBar_2 = new QToolBar(MainWindow);
        toolBar_2->setObjectName(QString::fromUtf8("toolBar_2"));
        toolBar_2->setIconSize(QSize(48, 48));
        MainWindow->addToolBar(Qt::LeftToolBarArea, toolBar_2);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Century Gothic")});
        font3.setPointSize(10);
        font3.setBold(false);
        statusBar->setFont(font3);
        MainWindow->setStatusBar(statusBar);
        toolBar_3 = new QToolBar(MainWindow);
        toolBar_3->setObjectName(QString::fromUtf8("toolBar_3"));
        toolBar_3->setEnabled(true);
        toolBar_3->setMovable(false);
        MainWindow->addToolBar(Qt::RightToolBarArea, toolBar_3);

        toolBar_2->addAction(actionConnect);
        toolBar_2->addAction(actionDisconnect);
        toolBar_2->addAction(actionConfigure);
        toolBar_2->addAction(actionQuit);
        toolBar_3->addSeparator();
        toolBar_3->addSeparator();
        toolBar_3->addSeparator();
        toolBar_3->addSeparator();
        toolBar_3->addSeparator();
        toolBar_3->addSeparator();

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);
        MB_Setpoint_Button->setDefault(false);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionConnect->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
#if QT_CONFIG(tooltip)
        actionConnect->setToolTip(QCoreApplication::translate("MainWindow", "Connect to a serial", nullptr));
#endif // QT_CONFIG(tooltip)
        actionDisconnect->setText(QCoreApplication::translate("MainWindow", "Disconnect", nullptr));
#if QT_CONFIG(tooltip)
        actionDisconnect->setToolTip(QCoreApplication::translate("MainWindow", "Disconnect from the serial", nullptr));
#endif // QT_CONFIG(tooltip)
        actionQuit->setText(QCoreApplication::translate("MainWindow", "Quit", nullptr));
#if QT_CONFIG(tooltip)
        actionQuit->setToolTip(QCoreApplication::translate("MainWindow", "Exit the program", nullptr));
#endif // QT_CONFIG(tooltip)
        actionConfigure->setText(QCoreApplication::translate("MainWindow", "Configure", nullptr));
#if QT_CONFIG(tooltip)
        actionConfigure->setToolTip(QCoreApplication::translate("MainWindow", "Configuration settings", nullptr));
#endif // QT_CONFIG(tooltip)
        label_32->setText(QCoreApplication::translate("MainWindow", "Gases:", nullptr));
        load_MFC4->setText(QCoreApplication::translate("MainWindow", "MFC4", nullptr));
        load_MFC3->setText(QCoreApplication::translate("MainWindow", "MFC3", nullptr));
        load_MFC2->setText(QCoreApplication::translate("MainWindow", "MFC2", nullptr));
        load_MFC1->setText(QCoreApplication::translate("MainWindow", "MFC1", nullptr));
        label_49->setText(QCoreApplication::translate("MainWindow", "RF Power:", nullptr));
        autotune_recipe->setText(QCoreApplication::translate("MainWindow", "OFF", nullptr));
        load_MB->setText(QCoreApplication::translate("MainWindow", "TUNER", nullptr));
        load_autotune->setText(QCoreApplication::translate("MainWindow", "AUTO TUNE", nullptr));
        load_RF->setText(QCoreApplication::translate("MainWindow", "RF", nullptr));
        label_87->setText(QCoreApplication::translate("MainWindow", "MB Tuner:", nullptr));
        label_103->setText(QCoreApplication::translate("MainWindow", "Plasma Recipe Settings", nullptr));
        label_89->setText(QCoreApplication::translate("MainWindow", "Z:", nullptr));
        cycles_recipe->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Century Gothic'; font-size:18px; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:18px;\">1</span></p></body></html>", nullptr));
        label_100->setText(QCoreApplication::translate("MainWindow", "Substrate:", nullptr));
        load_cycles->setText(QCoreApplication::translate("MainWindow", "CYCLES", nullptr));
        load_gap->setText(QCoreApplication::translate("MainWindow", "GAP", nullptr));
        load_speed->setText(QCoreApplication::translate("MainWindow", "SPEED", nullptr));
        Y1_set->setText(QCoreApplication::translate("MainWindow", "Y", nullptr));
        label_88->setText(QCoreApplication::translate("MainWindow", "Y:", nullptr));
        load_overlap->setText(QCoreApplication::translate("MainWindow", "OVERLAP", nullptr));
        load_thick->setText(QCoreApplication::translate("MainWindow", "THICKNESS", nullptr));
        x1_set->setText(QCoreApplication::translate("MainWindow", "X", nullptr));
        Y2_set->setText(QCoreApplication::translate("MainWindow", "Y2", nullptr));
        label_51->setText(QCoreApplication::translate("MainWindow", "X:", nullptr));
        load_autoscan->setText(QCoreApplication::translate("MainWindow", "AUTO SCAN", nullptr));
        x2_set->setText(QCoreApplication::translate("MainWindow", "X2", nullptr));
        label_102->setText(QCoreApplication::translate("MainWindow", "Substrate Dimensions:", nullptr));
        label_50->setText(QCoreApplication::translate("MainWindow", "Stage Position:", nullptr));
        label_95->setText(QCoreApplication::translate("MainWindow", "Stage:", nullptr));
        autoscan_recipe->setText(QCoreApplication::translate("MainWindow", "OFF", nullptr));
        save_recipe_button->setText(QCoreApplication::translate("MainWindow", "SAVE RECIPE", nullptr));
        label_31->setText(QCoreApplication::translate("MainWindow", "Recipe Name:", nullptr));
        load_recipe_button->setText(QCoreApplication::translate("MainWindow", "LOAD RECIPE", nullptr));
        name_recipe->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Century Gothic'; font-size:16px; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16px;\">Default Recipe</span></p></body></html>", nullptr));
        RFfor_label_2->setText(QCoreApplication::translate("MainWindow", " FORWARD PWR", nullptr));
        ref_label_3->setText(QCoreApplication::translate("MainWindow", "REFLECTED PWR", nullptr));
        MBfor_label_3->setText(QCoreApplication::translate("MainWindow", "MATCHBOX", nullptr));
        MB_Left_Button->setText(QCoreApplication::translate("MainWindow", "Left", nullptr));
        stepLabel->setText(QCoreApplication::translate("MainWindow", "Step Size", nullptr));
        stepSizeBox->setItemText(0, QCoreApplication::translate("MainWindow", "32", nullptr));
        stepSizeBox->setItemText(1, QCoreApplication::translate("MainWindow", "64", nullptr));
        stepSizeBox->setItemText(2, QCoreApplication::translate("MainWindow", "128", nullptr));

        MB_Right_Button->setText(QCoreApplication::translate("MainWindow", "Right", nullptr));
        ref_label_4->setText(QCoreApplication::translate("MainWindow", "TEMPERATURE (*C)", nullptr));
        gas1_label->setText(QCoreApplication::translate("MainWindow", "gas1", nullptr));
        gas1_actual_SLPM->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        gas1_recipe_SLPM->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        gas2_label->setText(QCoreApplication::translate("MainWindow", "gas1", nullptr));
        gas2_actual_SLPM->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        gas2_recipe_SLPM->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        gas3_label->setText(QCoreApplication::translate("MainWindow", "gas1", nullptr));
        gas3_actual_SLPM->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        gas3_recipe_SLPM->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        gas4_label->setText(QCoreApplication::translate("MainWindow", "gas1", nullptr));
        gas4_progressbar->setFormat(QCoreApplication::translate("MainWindow", "%p%", nullptr));
        gas4_actual_SLPM->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        gas4_recipe_SLPM->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        plsmaBtn->setText(QCoreApplication::translate("MainWindow", "Start Plasma", nullptr));
        label_52->setText(QCoreApplication::translate("MainWindow", "Stage Current Step Status:", nullptr));
        label_53->setText(QCoreApplication::translate("MainWindow", "Stage Next Step Status:", nullptr));
        vac_button->setText(QCoreApplication::translate("MainWindow", "VAC", nullptr));
        twospot_button->setText(QCoreApplication::translate("MainWindow", "TWO SPOT", nullptr));
        init_button->setText(QCoreApplication::translate("MainWindow", "INITIALIZE", nullptr));
        Joystick_button->setText(QCoreApplication::translate("MainWindow", "JOY", nullptr));
        label_55->setText(QCoreApplication::translate("MainWindow", "Wafer Diameter:", nullptr));
        wafer_diameter->setItemText(0, QCoreApplication::translate("MainWindow", "50", nullptr));
        wafer_diameter->setItemText(1, QCoreApplication::translate("MainWindow", "100", nullptr));
        wafer_diameter->setItemText(2, QCoreApplication::translate("MainWindow", "200", nullptr));
        wafer_diameter->setItemText(3, QCoreApplication::translate("MainWindow", "300", nullptr));

        label_54->setText(QCoreApplication::translate("MainWindow", "Stage Buttons", nullptr));
        scan_button->setText(QCoreApplication::translate("MainWindow", "SCAN", nullptr));
        diameter_button->setText(QCoreApplication::translate("MainWindow", "DIAMETER", nullptr));
        Home_button->setText(QCoreApplication::translate("MainWindow", "LOAD", nullptr));
        Stagepins_button->setText(QCoreApplication::translate("MainWindow", "PINS", nullptr));
        n2_purge_button->setText(QCoreApplication::translate("MainWindow", "N2 PURGE", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(Dashboard), QCoreApplication::translate("MainWindow", "Dashboard", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Cascade Recipe List", nullptr));
        remove_cascade_recipe_button->setText(QCoreApplication::translate("MainWindow", "Remove Selected", nullptr));
        add_cascade_recipe_button->setText(QCoreApplication::translate("MainWindow", "Add", nullptr));
        clear_cascade_recipe_button->setText(QCoreApplication::translate("MainWindow", "Clear All", nullptr));
        cascade_recipe_name->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Century Gothic'; font-size:16px; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16px;\">Default Recipe</span></p></body></html>", nullptr));
        load_cascade_recipe_button->setText(QCoreApplication::translate("MainWindow", "LOAD CASCADE RECIPE", nullptr));
        save_cascade_recipe_button->setText(QCoreApplication::translate("MainWindow", "SAVE CASCADE RECIPE", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(recipe), QCoreApplication::translate("MainWindow", "Cascade Recipe", nullptr));
        gas1_label_dup->setText(QCoreApplication::translate("MainWindow", "gas1", nullptr));
        gas1_setpoint_button_dup->setText(QCoreApplication::translate("MainWindow", "Enter", nullptr));
        gas1_actual_SLPM_dup->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        gas1_recipe_SLPM_dup->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        gas2_label_dup->setText(QCoreApplication::translate("MainWindow", "gas1", nullptr));
        gas2_setpoint_button_dup->setText(QCoreApplication::translate("MainWindow", "Enter", nullptr));
        gas2_actual_SLPM_dup->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        gas2_recipe_SLPM_dup->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        gas3_label_dup->setText(QCoreApplication::translate("MainWindow", "gas1", nullptr));
        gas3_setpoint_button_dup->setText(QCoreApplication::translate("MainWindow", "Enter", nullptr));
        gas3_actual_SLPM_dup->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        gas3_recipe_SLPM_dup->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        gas4_label_dup->setText(QCoreApplication::translate("MainWindow", "gas1", nullptr));
        gas4_progressbar_dup->setFormat(QCoreApplication::translate("MainWindow", "%p%", nullptr));
        gas4_setpoint_button_dup->setText(QCoreApplication::translate("MainWindow", "Enter", nullptr));
        gas4_actual_SLPM_dup->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        gas4_recipe_SLPM_dup->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        RFfor_label->setText(QCoreApplication::translate("MainWindow", " FORWARD PWR", nullptr));
        MBfor_label_2->setText(QCoreApplication::translate("MainWindow", "MATCHBOX", nullptr));
        ref_label->setText(QCoreApplication::translate("MainWindow", "REFLECTED PWR", nullptr));
        label_20->setText(QCoreApplication::translate("MainWindow", "C", nullptr));
        ref_label_2->setText(QCoreApplication::translate("MainWindow", "TEMPERATURE", nullptr));
        RF_Setpoint_Button->setText(QCoreApplication::translate("MainWindow", "Enter", nullptr));
        autoMode_Button_dup->setText(QCoreApplication::translate("MainWindow", "Auto Mode", nullptr));
        MB_Setpoint_Button->setText(QCoreApplication::translate("MainWindow", "Enter", nullptr));
        MB_Left_Button_dup->setText(QCoreApplication::translate("MainWindow", "Left", nullptr));
        MB_Right_Button_dup->setText(QCoreApplication::translate("MainWindow", "Right", nullptr));
        Heater_Button->setText(QCoreApplication::translate("MainWindow", "Toggle Heater", nullptr));
        plsmaBtn_dup->setText(QCoreApplication::translate("MainWindow", "Start Plasma", nullptr));
        Gasses_Label->setText(QCoreApplication::translate("MainWindow", "Gases", nullptr));
        Send_CMD_button->setText(QCoreApplication::translate("MainWindow", "Send CMD", nullptr));
        textRCVbox->setPlainText(QString());
        Clear_Button->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        CMDlabel->setText(QCoreApplication::translate("MainWindow", "Commands sent to CTL", nullptr));
        text2sendCMDbox->setPlainText(QString());
        text2sendCMDbox->setPlaceholderText(QString());
        RCVlabel->setText(QCoreApplication::translate("MainWindow", "Response from CTL", nullptr));
        textCMDbox->setPlainText(QString());
        textCMDbox->setPlaceholderText(QString());
        tabWidget->setTabText(tabWidget->indexOf(plasma), QCoreApplication::translate("MainWindow", "Plasma", nullptr));
        label_39->setText(QCoreApplication::translate("MainWindow", "Stage Variables:", nullptr));
        init_button_dup->setText(QCoreApplication::translate("MainWindow", "INITIALIZE", nullptr));
        Stagepins_button_dup->setText(QCoreApplication::translate("MainWindow", "PINS", nullptr));
        label_45->setText(QCoreApplication::translate("MainWindow", "Stage Buttons", nullptr));
        diameter_button_dup->setText(QCoreApplication::translate("MainWindow", "DIAMETER", nullptr));
        Home_button_dup->setText(QCoreApplication::translate("MainWindow", "LOAD", nullptr));
        twospot_button_dup->setText(QCoreApplication::translate("MainWindow", "TWO SPOT", nullptr));
        scan_button_dup->setText(QCoreApplication::translate("MainWindow", "SCAN", nullptr));
        vac_button_dup->setText(QCoreApplication::translate("MainWindow", "VAC", nullptr));
        n2_purge_button_dup->setText(QCoreApplication::translate("MainWindow", "N2 PURGE", nullptr));
        label_33->setText(QCoreApplication::translate("MainWindow", "Wafer Diameter:", nullptr));
        wafer_diameter_dup->setItemText(0, QCoreApplication::translate("MainWindow", "50", nullptr));
        wafer_diameter_dup->setItemText(1, QCoreApplication::translate("MainWindow", "100", nullptr));
        wafer_diameter_dup->setItemText(2, QCoreApplication::translate("MainWindow", "200", nullptr));
        wafer_diameter_dup->setItemText(3, QCoreApplication::translate("MainWindow", "300", nullptr));

        Joystick_button_dup->setText(QCoreApplication::translate("MainWindow", "JOY", nullptr));
        label_34->setText(QCoreApplication::translate("MainWindow", "X:", nullptr));
        label_84->setText(QCoreApplication::translate("MainWindow", "Y:", nullptr));
        label_85->setText(QCoreApplication::translate("MainWindow", "Z:", nullptr));
        label_43->setText(QCoreApplication::translate("MainWindow", "Thickness:", nullptr));
        label_42->setText(QCoreApplication::translate("MainWindow", "Gap:", nullptr));
        label_38->setText(QCoreApplication::translate("MainWindow", "Overlap:", nullptr));
        label_40->setText(QCoreApplication::translate("MainWindow", "Speed:", nullptr));
        label_41->setText(QCoreApplication::translate("MainWindow", "Cycles:", nullptr));
        label_47->setText(QCoreApplication::translate("MainWindow", "Y:", nullptr));
        label_35->setText(QCoreApplication::translate("MainWindow", "X:", nullptr));
        label_44->setText(QCoreApplication::translate("MainWindow", "X2:", nullptr));
        label_48->setText(QCoreApplication::translate("MainWindow", "Y2:", nullptr));
        label_46->setText(QCoreApplication::translate("MainWindow", "Substrate Dimensions:", nullptr));
        label_37->setText(QCoreApplication::translate("MainWindow", "Stage Position:", nullptr));
        label_24->setText(QCoreApplication::translate("MainWindow", "Stage Current Step Status:", nullptr));
        label_25->setText(QCoreApplication::translate("MainWindow", "Stage Next Step Status:", nullptr));
        door2->setText(QString());
        door1->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(axis), QCoreApplication::translate("MainWindow", "3 Axis Stage Controls", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "Tool Configuration", nullptr));
        heater_checkbox->setText(QCoreApplication::translate("MainWindow", "Heater Enabled", nullptr));
        ctldiagnostics_checkbox->setText(QCoreApplication::translate("MainWindow", "CTL Diagnostics Enabled", nullptr));
        mfc_loaded_checkbox->setText(QCoreApplication::translate("MainWindow", "Loaded Recipes Value Enabled", nullptr));
        batchID_checkBox->setText(QCoreApplication::translate("MainWindow", "Batch ID Enabled", nullptr));
        request_terminal->setText(QCoreApplication::translate("MainWindow", "Request/Response Terminal", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(settings), QCoreApplication::translate("MainWindow", "Tool Settings", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Access", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("MainWindow", "Recipe", nullptr));
        label_115->setText(QCoreApplication::translate("MainWindow", "RF Power:", nullptr));
        autotune_recipe_dup_3->setText(QCoreApplication::translate("MainWindow", "OFF", nullptr));
        load_RF_dup_3->setText(QCoreApplication::translate("MainWindow", "RF", nullptr));
        load_MB_dup_3->setText(QCoreApplication::translate("MainWindow", "TUNER", nullptr));
        label_116->setText(QCoreApplication::translate("MainWindow", "MB Tuner:", nullptr));
        load_autotune_dup_3->setText(QCoreApplication::translate("MainWindow", "AUTO TUNE", nullptr));
        label_117->setText(QCoreApplication::translate("MainWindow", "Stage:", nullptr));
        autoscan_recipe_dup_3->setText(QCoreApplication::translate("MainWindow", "OFF", nullptr));
        load_autoscan_dup_3->setText(QCoreApplication::translate("MainWindow", "AUTO SCAN", nullptr));
        load_cycles_dup_3->setText(QCoreApplication::translate("MainWindow", "CYCLES", nullptr));
        load_speed_dup_3->setText(QCoreApplication::translate("MainWindow", "SPEED", nullptr));
        load_thick_dup_3->setText(QCoreApplication::translate("MainWindow", "THICKNESS", nullptr));
        label_118->setText(QCoreApplication::translate("MainWindow", "Substrate:", nullptr));
        x1_set_dup_3->setText(QCoreApplication::translate("MainWindow", "X", nullptr));
        Y1_set_dup_3->setText(QCoreApplication::translate("MainWindow", "Y", nullptr));
        load_overlap_dup_3->setText(QCoreApplication::translate("MainWindow", "OVERLAP", nullptr));
        load_gap_dup_3->setText(QCoreApplication::translate("MainWindow", "GAP", nullptr));
        x2_set_dup_3->setText(QCoreApplication::translate("MainWindow", "X2", nullptr));
        Y2_set_dup_3->setText(QCoreApplication::translate("MainWindow", "Y2", nullptr));
        label_119->setText(QCoreApplication::translate("MainWindow", "Substrate Dimensions:", nullptr));
        label_120->setText(QCoreApplication::translate("MainWindow", "Stage Recipe Settings", nullptr));
        label_121->setText(QCoreApplication::translate("MainWindow", "Recipe Name:", nullptr));
        name_recipe_dup_3->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Century Gothic'; font-size:16px; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        label_122->setText(QCoreApplication::translate("MainWindow", "Plasma Recipe Settings", nullptr));
        save_recipe_button_dup_3->setText(QCoreApplication::translate("MainWindow", "SAVE RECIPE", nullptr));
        load_recipe_button_dup_3->setText(QCoreApplication::translate("MainWindow", "LOAD RECIPE", nullptr));
        label_123->setText(QCoreApplication::translate("MainWindow", "Gases:", nullptr));
        load_MFC1_dup_3->setText(QCoreApplication::translate("MainWindow", "MFC1", nullptr));
        load_MFC2_dup_3->setText(QCoreApplication::translate("MainWindow", "MFC2", nullptr));
        load_MFC4_dup_3->setText(QCoreApplication::translate("MainWindow", "MFC4", nullptr));
        load_MFC3_dup_3->setText(QCoreApplication::translate("MainWindow", "MFC3", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Page", nullptr));
        toolBar_2->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar_2", nullptr));
        toolBar_3->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar_3", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
