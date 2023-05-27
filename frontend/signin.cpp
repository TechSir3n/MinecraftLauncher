#include "./include/signin.hpp"

SignIn::SignIn(QWidget *parent) : QDialog(parent) {
  setupUI();
  manager = new QNetworkAccessManager(this);
  QObject::connect(manager, SIGNAL(finished(QNetworkReply *)), this,
                   SLOT(onNetworkManagerFinished(QNetworkReply *)));
}

SignIn::~SignIn() noexcept {
}

void SignIn::setupUI() {
  labelEmail = new QLabel(tr("Email"),this);
  labelPassword = new QLabel(tr("Password"),this);
  labelAccount = new QLabel(tr("I haven't account yet?"),this);
  labelTitle = new QLabel(tr("Login to MineLaucnh"),this);

  linePassword = new QLineEdit(this);
  lineEmail = new QLineEdit(this);

  buttonSubmit = new QPushButton(tr("Sign in"),this);

  QToolButton *toolButton = new QToolButton();
  toolButton->setIcon(
      QIcon("/home/ruslan/Documents/MineLaunch/resources/211661_eye_icon.png"));
  toolButton->setCursor(Qt::PointingHandCursor);

  QObject::connect(toolButton, &QToolButton::clicked, [=]() {
    if (linePassword->echoMode() == QLineEdit::Password) {
      linePassword->setEchoMode(QLineEdit::Normal);
    } else {
      linePassword->setEchoMode(QLineEdit::Password);
    }
  });

  QLabel *logo = new QLabel();
  QPixmap logoImage("/home/ruslan/Documents/MineLaunch/resources/u_ajax.png");
  logo->setPixmap(logoImage);
  logo->setAlignment(Qt::AlignCenter);
  logo->setFixedSize(230, 130);

  QHBoxLayout *hbox_layout = new QHBoxLayout;
  hbox_layout->addWidget(buttonSubmit);
  hbox_layout->setAlignment(Qt::AlignHCenter);

  QHBoxLayout *tool_layout = new QHBoxLayout;
  tool_layout->addWidget(toolButton);
  tool_layout->setAlignment(Qt::AlignRight);

  QHBoxLayout *centerLayout = new QHBoxLayout;
  centerLayout->addWidget(logo, 0, Qt::AlignCenter);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->insertLayout(0, centerLayout);
  layout->insertWidget(1, labelTitle);
  layout->addWidget(labelEmail);
  layout->addWidget(lineEmail);
  layout->addSpacing(10);
  layout->addWidget(labelPassword);
  layout->addWidget(linePassword);
  layout->addSpacing(10);
  layout->addLayout(tool_layout);
  layout->addLayout(hbox_layout);
  layout->setContentsMargins(50, 20, 50, 20);
  layout->setSpacing(5);

  QFont titleFont("Roboto", 20, QFont::Bold);
  QFont labelFont("Open Sans", 14);

  labelPassword->setFont(labelFont);
  labelEmail->setFont(labelFont);
  labelTitle->setFont(titleFont);
  labelTitle->setAlignment(Qt::AlignCenter);

  linePassword->setEchoMode(QLineEdit::Password);
  linePassword->setPlaceholderText("Enter password");
  lineEmail->setPlaceholderText("Enter email");

  linePassword->setStyleSheet(lineEditStyle);
  lineEmail->setStyleSheet(lineEditStyle);

  auto result = SignUp::getInstance().CalculateCenterMonitor();
  QIcon icon("/home/ruslan/Documents/MineLaunch/resources/u_ajax.png");

  this->setWindowIcon(icon);
  this->setFixedSize(550, 700);
  this->move(std::get<0>(result), std::get<1>(result));


  QObject::connect(buttonSubmit, &QPushButton::clicked, [&]() {
      const QString password = linePassword->text();
      const QString email = lineEmail->text();
      sendBackend(password, email);
  });
}

void SignIn::sendBackend(const QString &pass, const QString &email) {
  QUrl url("http://localhost:8080/signin");

  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    "application/x-www-form-urlencoded");

  QUrlQuery postData;
  postData.addQueryItem("password", pass);
  postData.addQueryItem("email", email);

  manager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
}

void SignIn::onNetworkManagerFinished(QNetworkReply *reply) {
  int statusCode =
      reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
  switch (statusCode) {
  case 200:
    logger.log(LogLevel::Info, "Registration completed successfuly");
    break;
  case 400:
    logger.log(LogLevel::Error, "Registration error: ");
    QMessageBox::warning(this, "Error",
                         "Error registration,status code: " +
                             QString::number(statusCode));
    break;

  default:
    logger.log(LogLevel::Error, "Unknown registration error,status code: " +
                                    std::to_string(statusCode));
    break;
  }
  reply->deleteLater();
}