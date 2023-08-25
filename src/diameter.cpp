#include "include/diameter.h"

const int NUM_WAFER_SIZES = 5;

Diameter::Diameter()
{
    m_pWaferDiameter = new int[NUM_WAFER_SIZES] {50, 100, 150, 200, 300};
    m_WaferDiameterText = (QStringList() << "50mm" << "100mm" << "150mm" << "200mm" << "300mm");
}

Diameter::~Diameter()
{
    delete m_pWaferDiameter;
}

int Diameter::getNumWaferSizes() const
{
    return NUM_WAFER_SIZES;
}
//void MainWindow::RunDiameter() { //calculate scan recipe based on wafer diameter selected
//    double wafer_diameter;
//    double radius;
//    double val;
//    bool ok;
//    //find the radius
//    wafer_diameter = ui->wafer_diameter->currentText().toDouble(&ok);
//    radius = wafer_diameter / 2.0;
//    //find the points defining the box
//    ScanSM.setX1(stageBaseCoordinates.getXp2Base() - radius);
//    ScanSM.setX2(stageBaseCoordinates.getXp2Base() + radius);
//    ScanSM.setY1(stageBaseCoordinates.getY() - radius);
//    ScanSM.setY2(stageBaseCoordinates.getY() + radius);

//    //update the "actual" display after coord sys translation
//    val = XPos_RefB_2_RefPH(ScanSM.getX1());
//    ui->x1_recipe->setText(QString::number(val));
//    val = XPos_RefB_2_RefPH(ScanSM.getX2());
//    ui->x2_recipe->setText(QString::number(val));

//    val = YPos_RefB_2_RefPH(ScanSM.getY1());
//    ui->y1_recipe->setText(QString::number(val));
//    val = YPos_RefB_2_RefPH(ScanSM.getY2());
//    ui->y2_recipe->setText(QString::number(val));


//}
