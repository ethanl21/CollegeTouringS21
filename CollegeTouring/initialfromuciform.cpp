#include "initialfromuciform.h"
#include "ui_initialfromuciform.h"

initialFromUciForm::initialFromUciForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::initialFromUciForm)
{

    totalDistance = 0;
    currentCampus.name = "University of California, Irvine (UCI)";
    currentCampus.number = 0;
    campusCount = 10;
    viewCounter = 0;

    const QString FILE_NAME = "collegetouring.db";
    QString dbPath = qApp->applicationDirPath();
    dbPath.append('/' + FILE_NAME);

    // open database with file path
    database = new DbManager(dbPath);

    ui->setupUi(this);
}

initialFromUciForm::~initialFromUciForm()
{
    delete database;
    delete ui;
}

void initialFromUciForm::on_cancelTripButton_clicked()
{
    close(); // close this window
}

void initialFromUciForm::on_beginTripButton_clicked()
{
    vector<nameNumber> potentialVisits; // holds all campuses that can be visited from the current campus
    bool alreadyVisited = false;        // whether the current campus has already been visited in this route
    QString temp;                       // used to change labels

    // route begins at UCI
    route.push_back(currentCampus);

    // determine the complete route
    for (int i = 1; i <= campusCount; i++)
    {
        // collect all campuses that can be traveled to from currentCampus
        potentialVisits = database->getDistances(currentCampus.name);
        currentCampus = potentialVisits[0];

        // find the nearest campus from all potential visits
        for (int j = 0; j <= campusCount - 1; j++)
        {
            // check that the current campus has not already been previously visited in the route
            for (campusIterator = route.begin(); campusIterator < route.end(); campusIterator++)
            {
                if (potentialVisits[j].name == (*campusIterator).name)
                {
                    alreadyVisited = true;
                }
            }

            if (potentialVisits[j].number <= currentCampus.number && !alreadyVisited)
            {
                // currentCampus holds the closest potential visit
                currentCampus = potentialVisits[j];
            }

            alreadyVisited = false;
        }

        // next in route becomes the closest potential visit
        route.push_back(currentCampus);
        // add to the total distance
        totalDistance += currentCampus.number;
    }

    // begin viewing starting college in route

    on_nextButton_clicked();

    ui->initialFromUciStacked->setCurrentIndex(1);

}

void initialFromUciForm::on_exitButton_clicked()
{
    close(); // close this window
}

void initialFromUciForm::on_nextButton_clicked()
{
    // moves to next campus in the route while available
    if (viewCounter <= campusCount)
    {
        ui->welcomeLabel->setText(route[viewCounter].name);

        ui->nextLabel->setText("Next campus: ");
        if (viewCounter < campusCount)
        {
            ui->nextVarLabel->setText(route[viewCounter + 1].name);
        }
        else
        {
            ui->nextVarLabel->setText("N/A");
        }

        ui->previousLabel->setText("Previous campus: ");
        if (viewCounter > 0)
        {
           ui->previousVarLabel->setText(route[viewCounter - 1].name);
        }
        else
        {
            ui->previousVarLabel->setText("N/A");
        }

        ui->distanceLabel->setText("Distance from previous campus: ");
        ui->distanceVarLabel->setText(QString::number(route[viewCounter].number));

        viewCounter++;
        if (viewCounter > campusCount)
        {
            ui->nextButton->setText("Finish Trip");
        }
    }
    // when all campuses have been seen, change window to summary page
    else
    {
        ui->nextButton->setVisible(false);
        ui->souvenirsButton->setVisible(false);
        ui->exitButton->setText("Exit");
        ui->welcomeLabel->setText("Route Summary");

        ui->nextLabel->setText("Starting campus: ");
        ui->nextVarLabel->setText(route[0].name);

        ui->previousLabel->setText("Ending campus: ");
        ui->previousVarLabel->setText(route[campusCount].name);

        ui->distanceLabel->setText("Total distance traveled: ");
        ui->distanceVarLabel->setText(QString::number(totalDistance));

    }

}