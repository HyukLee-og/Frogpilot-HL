#include <QDir>
#include <QRegularExpression>
#include <QTextStream>

#include "selfdrive/frogpilot/ui/vehicle_settings.h"

QStringList getCarNames(const QString &carMake) {
  QMap<QString, QString> makeMap;
  makeMap["acura"] = "honda";
  makeMap["audi"] = "volkswagen";
  makeMap["buick"] = "gm";
  makeMap["cadillac"] = "gm";
  makeMap["chevrolet"] = "gm";
  makeMap["chrysler"] = "chrysler";
  makeMap["dodge"] = "chrysler";
  makeMap["ford"] = "ford";
  makeMap["gm"] = "gm";
  makeMap["gmc"] = "gm";
  makeMap["genesis"] = "hyundai";
  makeMap["honda"] = "honda";
  makeMap["hyundai"] = "hyundai";
  makeMap["infiniti"] = "nissan";
  makeMap["jeep"] = "chrysler";
  makeMap["kia"] = "hyundai";
  makeMap["lexus"] = "toyota";
  makeMap["lincoln"] = "ford";
  makeMap["man"] = "volkswagen";
  makeMap["mazda"] = "mazda";
  makeMap["nissan"] = "nissan";
  makeMap["ram"] = "chrysler";
  makeMap["seat"] = "volkswagen";
  makeMap["subaru"] = "subaru";
  makeMap["tesla"] = "tesla";
  makeMap["toyota"] = "toyota";
  makeMap["volkswagen"] = "volkswagen";
  makeMap["skoda"] = "volkswagen";

  QString dirPath = "../../selfdrive/car";
  QDir dir(dirPath);
  QString targetFolder = makeMap.value(carMake, carMake);
  QStringList names;

  foreach (const QString &folder, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
    if (folder == targetFolder) {
      QFile file(dirPath + "/" + folder + "/values.py");
      if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QRegularExpression regex("class CAR\\(StrEnum\\):([\\s\\S]*?)(?=^\\w)", QRegularExpression::MultilineOption);
        QRegularExpressionMatch match = regex.match(QTextStream(&file).readAll());
        file.close();

        if (match.hasMatch()) {
          QRegularExpression nameRegex("=\\s*\"([^\"]+)\"");
          QRegularExpressionMatchIterator it = nameRegex.globalMatch(match.captured(1));
          while (it.hasNext()) {
            names << it.next().captured(1);
          }
        }
      }
    }
  }

  std::sort(names.begin(), names.end());
  return names;
}

FrogPilotVehiclesPanel::FrogPilotVehiclesPanel(SettingsWindow *parent) : FrogPilotListWidget(parent) {
  selectMakeButton = new ButtonControl(tr("Select Make"), tr("SELECT"));
  QObject::connect(selectMakeButton, &ButtonControl::clicked, [this]() {
    std::string currentMake = params.get("CarMake");
    QStringList makes = {
      "Acura", "Audi", "BMW", "Buick", "Cadillac", "Chevrolet", "Chrysler", "Dodge", "Ford", "GM", "GMC",
      "Genesis", "Honda", "Hyundai", "Infiniti", "Jeep", "Kia", "Lexus", "Lincoln", "MAN", "Mazda",
      "Mercedes", "Nissan", "Ram", "SEAT", "Subaru", "Tesla", "Toyota", "Volkswagen", "Volvo", "Škoda",
    };

    QString newMakeSelection = MultiOptionDialog::getSelection(tr("Select a Make"), makes, QString::fromStdString(currentMake), this);
    if (!newMakeSelection.isEmpty()) {
      carMake = newMakeSelection;
      params.putNonBlocking("CarMake", carMake.toStdString());
      selectMakeButton->setValue(newMakeSelection);
      setModels();
    }
  });
  addItem(selectMakeButton);

  selectModelButton = new ButtonControl(tr("Select Model"), tr("SELECT"));
  QString modelSelection = QString::fromStdString(params.get("CarModel"));
  QObject::connect(selectModelButton, &ButtonControl::clicked, [this]() {
    std::string currentModel = params.get("CarModel");
    QString newModelSelection = MultiOptionDialog::getSelection(tr("Select a Model"), models, QString::fromStdString(currentModel), this);
    if (!newModelSelection.isEmpty()) {
      params.putNonBlocking("CarModel", newModelSelection.toStdString());
      selectModelButton->setValue(newModelSelection);
    }
  });
  selectModelButton->setValue(modelSelection);
  addItem(selectModelButton);
  selectModelButton->setVisible(false);

  ParamControl *forceFingerprint = new ParamControl("ForceFingerprint", "Disable Automatic Fingerprint Detection", "Forces the selected fingerprint and prevents it from ever changing.", "", this);
  addItem(forceFingerprint);

  std::vector<std::tuple<QString, QString, QString, QString>> vehicleToggles {
    {"EVTable", "EV Lookup Tables", "Smoothen out the gas and brake controls for EV vehicles.", ""},
    {"GasRegenCmd", "GM Truck Gas Tune", "Increase acceleration and smoothen brake to stop. For use on Silverado/Sierra only.", ""},
    {"LongPitch", "Long Pitch Compensation", "Reduce speed and acceleration error for greater passenger comfort and improved vehicle efficiency.", ""},
    {"LowerVolt", "Lower Volt Enable Speed", "Lower the Volt's minimum enable speed to enable openpilot at any speed.", ""},

    {"CrosstrekTorque", "Subaru Crosstrek Torque Increase", "Increases the maximum allowed torque for the Subaru Crosstrek.", ""},

    {"LockDoors", "Lock Doors In Drive", "Automatically lock the doors when in drive and unlock when in park.", ""},
    {"LongitudinalTune", "Longitudinal Tune", "Use a custom Toyota longitudinal tune.", ""},
    {"SNGHack", "Stop and Go Hack", "Enable the 'Stop and Go' hack for vehicles without stock stop and go functionality.", ""},
  };

  for (const auto &[param, title, desc, icon] : vehicleToggles) {
    ParamControl *toggle;

    if (param == "LongitudinalTune") {
      std::vector<std::pair<QString, QString>> tuneOptions{
        {"StockTune", tr("Stock")},
        {"CydiaTune", tr("Cydia's")},
        {"DragonPilotTune", tr("DragonPilot's")},
        {"FrogsGoMooTune", tr("FrogPilot's")},
      };
      toggle = new FrogPilotButtonsParamControl(param, title, desc, icon, tuneOptions);

      QObject::connect(static_cast<FrogPilotButtonsParamControl*>(toggle), &FrogPilotButtonsParamControl::buttonClicked, [this]() {
        if (started) {
          if (FrogPilotConfirmationDialog::toggle("Reboot required to take effect.", "Reboot Now", this)) {
            Hardware::reboot();
          }
        }
      });

    } else {
      toggle = new ParamControl(param, title, desc, icon, this);
    }

    addItem(toggle);
    toggles[param.toStdString()] = toggle;

    QObject::connect(toggle, &ToggleControl::toggleFlipped, [this]() {
      updateToggles();
    });

    QObject::connect(toggle, &AbstractControl::showDescriptionEvent, [this]() {
      update();
    });
  }

  std::set<std::string> rebootKeys = {"CrosstrekTorque", "GasRegenCmd", "LowerVolt"};
  for (const std::string &key : rebootKeys) {
    QObject::connect(toggles[key], &ToggleControl::toggleFlipped, [this]() {
      if (started) {
        if (FrogPilotConfirmationDialog::toggle("Reboot required to take effect.", "Reboot Now", this)) {
          Hardware::reboot();
        }
      }
    });
  }

  QObject::connect(uiState(), &UIState::offroadTransition, this, [this](bool offroad) {
    if (!offroad) {
      std::thread([this]() {
        while (carMake.isEmpty()) {
          std::this_thread::sleep_for(std::chrono::seconds(1));
          carMake = QString::fromStdString(params.get("CarMake"));
        }
        setModels();
      }).detach();
    }
  });

  carMake = QString::fromStdString(params.get("CarMake"));
  if (!carMake.isEmpty()) {
    setModels();
  }

  QObject::connect(uiState(), &UIState::uiUpdate, this, &FrogPilotVehiclesPanel::updateState);
}

void FrogPilotVehiclesPanel::updateState(const UIState &s) {
  started = s.scene.started;
}

void FrogPilotVehiclesPanel::updateToggles() {
  std::thread([this]() {
    paramsMemory.putBool("FrogPilotTogglesUpdated", true);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    paramsMemory.putBool("FrogPilotTogglesUpdated", false);
  }).detach();
}

void FrogPilotVehiclesPanel::setModels() {
  models = getCarNames(carMake.toLower());
  setToggles();
}

void FrogPilotVehiclesPanel::setToggles() {
  selectMakeButton->setValue(carMake);
  selectModelButton->setVisible(!carMake.isEmpty());

  bool gm = carMake == "Buick" || carMake == "Cadillac" || carMake == "Chevrolet" || carMake == "GM" || carMake == "GMC";
  bool subaru = carMake == "Subaru";
  bool toyota = carMake == "Lexus" || carMake == "Toyota";

  for (auto &[key, toggle] : toggles) {
    if (toggle) {
      toggle->setVisible(false);

      if (gm) {
        toggle->setVisible(gmKeys.find(key.c_str()) != gmKeys.end());
      } else if (subaru) {
        toggle->setVisible(subaruKeys.find(key.c_str()) != subaruKeys.end());
      } else if (toyota) {
        toggle->setVisible(toyotaKeys.find(key.c_str()) != toyotaKeys.end());
      }
    }
  }

  update();
}
