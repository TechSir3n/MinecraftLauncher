#pragma once

#include "signup.hpp"
#include <QAction>
#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QDialog>
#include <QFileInfo>
#include <QGroupBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QListWidget>
#include <QMap>
#include <QMenu>
#include <QMenuBar>
#include <QFrame>
#include <QPalette>
#include <QPushButton>
#include <QRadioButton>
#include <QRegularExpression>
#include <QSlider>
#include <QStringList>
#include <QTabWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QtCore>

struct Mod {
  QString name;
  QString description;
  QString file;
};

class DashBoard : public QDialog {
  Q_OBJECT
public:
  explicit DashBoard(QWidget *parent = nullptr);

  ~DashBoard();

public:
  static DashBoard &getInstance() {
    static DashBoard instance;
    return instance;
  }

private slots:
  void searchModsByName();

  void onSliderValueChanged(int value);

private:
  void initalizeUI() noexcept;

  void loadMods() noexcept;

  void loadVersionsMinecraft() noexcept;

  void addGameTab() noexcept;

  void addMenuTab() noexcept;

  void addSettings() noexcept;

  void addListServers() noexcept;

private:
  QPushButton *play;
  QPushButton *update;
  QPushButton *save;
  QPushButton *reset;
  QPushButton *cancel;
  QPushButton *search;
  QPushButton *download;

  QTableWidget *modsTable;
  QTabWidget *tabWidget;

  QLineEdit *editSearch;
  QVector<Mod> loadedMods;

  QComboBox *versionSelector;
  QComboBox *screenExtension;
  QComboBox *qualityGraphic;
  QComboBox* choiceLanguage;

  QSlider *soundSlider;
  QSlider *brightnessSlider;
  QSlider *choiceColor;

  QMenuBar *menuBar;
  QRadioButton *fullScreen;
  QRadioButton *windowMode;
  QButtonGroup *buttonGroup;

  QGroupBox *groupBoxGame;
  QGroupBox *groupBoxLauncher;

private:
  Logger logger;
};
