#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QMessageBox>
#include <QScrollBar>
#include <QResizeEvent>
#include <QFont>
#include <QPalette>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QColor>
#include <QRandomGenerator>

static const QColor BG("#090b0e");
static const QColor PANEL("#11151a");
static const QColor LINE("#252c34");
static const QColor TEXT("#f5f7fa");
static const QColor MUTED("#aab3bd");
static const QColor ACCENT("#a7f04b");

class ParticleBackground : public QWidget {
public:
    explicit ParticleBackground(QWidget *parent=nullptr) : QWidget(parent) {
        setMinimumHeight(520);
        setAttribute(Qt::WA_StyledBackground, true);
    }
protected:
    void paintEvent(QPaintEvent *) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.fillRect(rect(), BG);

        QLinearGradient bg(0, 0, width(), height());
        bg.setColorAt(0.0, QColor("#090b0e"));
        bg.setColorAt(0.52, QColor("#111a19"));
        bg.setColorAt(1.0, QColor("#090b0e"));
        p.fillRect(rect(), bg);

        QRadialGradient glow(width()*0.78, height()*0.30, width()*0.62);
        glow.setColorAt(0.0, QColor(71, 91, 74, 135));
        glow.setColorAt(0.35, QColor(27, 48, 41, 95));
        glow.setColorAt(1.0, QColor(9, 11, 14, 0));
        p.fillRect(rect(), glow);

        p.setPen(Qt::NoPen);
        for (int y=10; y<height(); y+=23) {
            for (int x=10; x<width(); x+=23) {
                int alpha = (x > width()*0.38) ? 30 : 12;
                p.setBrush(QColor(255,255,255,alpha));
                p.drawEllipse(QPointF(x,y), 0.8, 0.8);
            }
        }
        QLinearGradient shade(0, 0, width(), 0);
        shade.setColorAt(0, QColor(5,7,9,245));
        shade.setColorAt(0.55, QColor(5,7,9,155));
        shade.setColorAt(1, QColor(5,7,9,65));
        p.fillRect(rect(), shade);
    }
};

static QLabel *makeLabel(const QString &text, int size, QColor color=TEXT, bool bold=false) {
    auto *l = new QLabel(text);
    QFont f("Inter", size);
    f.setBold(bold);
    l->setFont(f);
    l->setStyleSheet(QString("color:%1; background:transparent;").arg(color.name()));
    l->setWordWrap(true);
    return l;
}
static QPushButton *makeButton(const QString &text, bool primary=false) {
    auto *b = new QPushButton(text);
    b->setCursor(Qt::PointingHandCursor);
    b->setMinimumHeight(46);
    b->setStyleSheet(primary
        ? "QPushButton{background:#a7f04b;color:#10140b;border:1px solid #a7f04b;border-radius:8px;padding:0 20px;font-weight:800;} QPushButton:hover{background:#b8ff60;}"
        : "QPushButton{background:#ffffff09;color:#f5f7fa;border:1px solid #252c34;border-radius:8px;padding:0 20px;font-weight:800;} QPushButton:hover{background:#252c34;}");
    return b;
}
static QFrame *makeCard(const QString &number, const QString &title, const QString &body) {
    auto *card = new QFrame;
    card->setStyleSheet("QFrame{background:qlineargradient(x1:0,y1:0,x2:1,y2:1,stop:0 #151a20,stop:1 #0e1115);border:1px solid #252c34;border-radius:14px;}");
    auto *v = new QVBoxLayout(card);
    v->setContentsMargins(24,24,24,24);
    v->setSpacing(10);
    v->addWidget(makeLabel(number, 12, ACCENT, true));
    v->addWidget(makeLabel(title, 19, TEXT, true));
    v->addWidget(makeLabel(body, 14, MUTED));
    v->addStretch();
    card->setMinimumHeight(190);
    return card;
}

class LosSantosWindow : public QMainWindow {
    QScrollArea *scroll = nullptr;
    QWidget *content = nullptr;
    QWidget *playSection = nullptr;
public:
    LosSantosWindow() {
        setWindowTitle("Los Santos — Browser Gaming");
        resize(1180, 900);
        setMinimumSize(360, 600);
        setStyleSheet("QMainWindow{background:#090b0e;} QWidget{color:#f5f7fa;}");

        scroll = new QScrollArea;
        scroll->setWidgetResizable(true);
        scroll->setFrameShape(QFrame::NoFrame);
        scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scroll->setStyleSheet("QScrollArea{background:#090b0e;border:0;} QScrollBar:vertical{background:#090b0e;width:9px;margin:0;} QScrollBar::handle:vertical{background:#343c44;border-radius:4px;min-height:30px;} QScrollBar::add-line:vertical,QScrollBar::sub-line:vertical{height:0;}");
        content = new QWidget;
        content->setStyleSheet("background:#090b0e;");
        auto *root = new QVBoxLayout(content);
        root->setContentsMargins(0,0,0,0);
        root->setSpacing(0);

        // Navigation
        auto *nav = new QWidget;
        nav->setMinimumHeight(72);
        nav->setMaximumHeight(72);
        nav->setStyleSheet("background:#090b0e;border-bottom:1px solid #20252b;");
        auto *navRow = new QHBoxLayout(nav);
        navRow->setContentsMargins(24,0,24,0);
        auto *brand = makeLabel("LOS ", 16, TEXT, true);
        auto *brandGreen = makeLabel("SANTOS", 16, ACCENT, true);
        auto *web = makeLabel(" / WEB", 16, TEXT, true);
        auto *brandRow = new QHBoxLayout;
        brandRow->setSpacing(0);
        brandRow->addWidget(brand); brandRow->addWidget(brandGreen); brandRow->addWidget(web);
        auto *brandWrap = new QWidget;
        brandWrap->setLayout(brandRow);
        navRow->addWidget(brandWrap);
        navRow->addStretch();
        navRow->addWidget(makeLabel("COMMUNITY GAMING CONCEPT", 11, MUTED, true));
        navRow->addSpacing(22);
        auto *playNav = makeButton("PLAY AREA  ↘");
        playNav->setMinimumHeight(38);
        playNav->setMaximumHeight(38);
        navRow->addWidget(playNav);
        connect(playNav, &QPushButton::clicked, this, [this]{ goToPlay(); });
        root->addWidget(nav);

        // Hero section
        auto *hero = new ParticleBackground;
        auto *heroLayout = new QVBoxLayout(hero);
        heroLayout->setContentsMargins(28,70,28,64);
        heroLayout->setSpacing(0);
        auto *heroInner = new QWidget;
        heroInner->setMaximumWidth(1120);
        auto *hv = new QVBoxLayout(heroInner);
        hv->setContentsMargins(0,0,0,0);
        hv->setSpacing(0);
        hv->addWidget(makeLabel("OPEN WORLD  •  BROWSER-FIRST CONCEPT", 12, ACCENT, true));
        hv->addSpacing(16);
        auto *headline = makeLabel("YOUR NEXT\nADVENTURE", 58, TEXT, true);
        headline->setStyleSheet("color:#f5f7fa;background:transparent;font-weight:900;letter-spacing:-1px;");
        headline->setMinimumHeight(128);
        hv->addWidget(headline);
        hv->addSpacing(12);
        hv->addWidget(makeLabel("A cinematic home for browser gaming. Start from one clean page, check your setup, and connect to a game stream you’re authorized to use.", 18, QColor("#c2c9d0")));
        hv->itemAt(hv->count()-1)->widget()->setMaximumWidth(590);
        hv->addSpacing(26);
        auto *actions = new QHBoxLayout;
        actions->setSpacing(12);
        auto *heroPlay = makeButton("▶   PLAY NOW", true);
        auto *how = makeButton("HOW IT WORKS  ↓");
        actions->addWidget(heroPlay); actions->addWidget(how); actions->addStretch();
        hv->addLayout(actions);
        connect(heroPlay, &QPushButton::clicked, this, [this]{ goToPlay(); });
        connect(how, &QPushButton::clicked, this, [this]{ scrollToWidget(findChild<QWidget*>("howSection")); });
        heroLayout->addWidget(heroInner, 0, Qt::AlignHCenter);
        root->addWidget(hero);

        // How it works
        auto *howSection = new QWidget;
        howSection->setObjectName("howSection");
        auto *howLayout = new QVBoxLayout(howSection);
        howLayout->setContentsMargins(28,58,28,58);
        howLayout->setSpacing(22);
        auto *sectionHead = new QHBoxLayout;
        auto *headLeft = new QVBoxLayout;
        headLeft->addWidget(makeLabel("THE SETUP", 12, ACCENT, true));
        headLeft->addWidget(makeLabel("Built for a smooth start", 28, TEXT, true));
        sectionHead->addLayout(headLeft);
        sectionHead->addStretch();
        sectionHead->addWidget(makeLabel("01 — 03", 13, MUTED));
        howLayout->addLayout(sectionHead);
        auto *cards = new QGridLayout;
        cards->setSpacing(16);
        cards->addWidget(makeCard("01 / CONNECT", "Use an authorized stream", "Connect a supported cloud gaming service or your own licensed PC streaming session. This page does not host GTA V files."),0,0);
        cards->addWidget(makeCard("02 / CHECK", "Test your connection", "Stable broadband and a nearby streaming server help reduce input delay. Wi-Fi quality and network congestion still matter."),0,1);
        cards->addWidget(makeCard("03 / PLAY", "Enter fullscreen", "When a compatible stream is connected, use its controls and fullscreen option for the best browser experience."),0,2);
        howLayout->addLayout(cards);
        root->addWidget(howSection);

        // Play area
        playSection = new QWidget;
        playSection->setObjectName("playSection");
        auto *playLayout = new QVBoxLayout(playSection);
        playLayout->setContentsMargins(28,0,28,62);
        playLayout->setSpacing(22);
        auto *playHead = new QHBoxLayout;
        auto *playHeadLeft = new QVBoxLayout;
        playHeadLeft->addWidget(makeLabel("GAME SESSION", 12, ACCENT, true));
        playHeadLeft->addWidget(makeLabel("Launch area", 28, TEXT, true));
        playHead->addLayout(playHeadLeft);
        playHead->addStretch();
        auto *status = new QLabel("●  STREAM NOT CONNECTED");
        status->setStyleSheet("QLabel{background:#182016;color:#c9f6a3;border:1px solid #34402a;border-radius:16px;padding:7px 11px;font-size:12px;font-weight:800;}");
        playHead->addWidget(status);
        playLayout->addLayout(playHead);

        auto *box = new QFrame;
        box->setStyleSheet("QFrame{background:#0d1116;border:1px solid #252c34;border-radius:16px;}");
        auto *boxLayout = new QGridLayout(box);
        boxLayout->setContentsMargins(28,28,28,28);
        boxLayout->setSpacing(24);
        auto *left = new QWidget;
        auto *leftLayout = new QVBoxLayout(left);
        leftLayout->setContentsMargins(0,0,0,0);
        leftLayout->setSpacing(12);
        leftLayout->addWidget(makeLabel("Ready when your stream is", 24, TEXT, true));
        leftLayout->addWidget(makeLabel("This starter website is hosted separately from the game. Add your authorized provider’s launch URL or embed integration here once you have one that permits browser access.", 14, MUTED));
        auto *note = new QLabel("<b>Heads up:</b> Free static hosting can serve this website, but it cannot provide the GPU server needed to run GTA V. A free, always-available GTA V stream is not guaranteed.");
        note->setWordWrap(true);
        note->setStyleSheet("QLabel{background:#19170f;border:1px solid #4a3d1d;border-radius:10px;padding:14px;color:#e8d8a6;font-size:14px;}");
        leftLayout->addWidget(note);
        leftLayout->addStretch();

        auto *screen = new QFrame;
        screen->setMinimumHeight(260);
        screen->setStyleSheet("QFrame{background:qradialgradient(cx:0.5,cy:0.45,radius:0.8,stop:0 #202a23,stop:1 #0a0d10);border:1px solid #252d32;border-radius:12px;}");
        auto *screenLayout = new QVBoxLayout(screen);
        screenLayout->setAlignment(Qt::AlignCenter);
        screenLayout->setSpacing(10);
        screenLayout->addWidget(makeLabel("⌁", 34, ACCENT, true),0,Qt::AlignHCenter);
        screenLayout->addWidget(makeLabel("Waiting for a game stream", 20, TEXT, true),0,Qt::AlignHCenter);
        auto *screenText = makeLabel("No stream URL is configured yet. Once you choose a legitimate service, connect it here using that provider’s supported method.", 14, MUTED);
        screenText->setAlignment(Qt::AlignCenter);
        screenText->setMaximumWidth(360);
        screenLayout->addWidget(screenText,0,Qt::AlignHCenter);
        auto *check = makeButton("VIEW SETUP CHECKLIST", true);
        screenLayout->addWidget(check,0,Qt::AlignHCenter);
        connect(check, &QPushButton::clicked, this, [this]{
            QMessageBox::information(this, "Setup checklist",
                "Setup checklist:\n"
                "1. Choose a legitimate cloud gaming or remote-play service.\n"
                "2. Confirm it supports your GTA V license and browser/PC setup.\n"
                "3. Add only its official supported launch/embed flow.\n"
                "4. Test network latency and fullscreen.\n\n"
                "This template does not include GTA V or a game-streaming backend.");
        });

        boxLayout->addWidget(left,0,0);
        boxLayout->addWidget(screen,0,1);
        boxLayout->setColumnStretch(0,6);
        boxLayout->setColumnStretch(1,5);
        playLayout->addWidget(box);
        root->addWidget(playSection);

        auto *footer = new QWidget;
        footer->setStyleSheet("background:#090b0e;border-top:1px solid #252c34;");
        auto *footerLayout = new QHBoxLayout(footer);
        footerLayout->setContentsMargins(28,26,28,26);
        footerLayout->addWidget(makeLabel("LOS SANTOS / WEB — starter template", 12, MUTED));
        footerLayout->addStretch();
        auto *disclaimer = makeLabel("Unofficial fan concept. Not affiliated with Rockstar Games or Take-Two Interactive.", 12, MUTED);
        disclaimer->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        footerLayout->addWidget(disclaimer);
        root->addWidget(footer);

        scroll->setWidget(content);
        setCentralWidget(scroll);
    }

protected:
    void resizeEvent(QResizeEvent *event) override {
        QMainWindow::resizeEvent(event);
        if (!content) return;
        const bool compact = event->size().width() < 760;
        // Keep the same layout structure; adjust typography and spacing on narrow windows.
        for (auto *label : content->findChildren<QLabel*>()) {
            if (label->text() == "YOUR NEXT\nADVENTURE") {
                QFont f = label->font();
                f.setPointSize(compact ? 39 : 58);
                label->setFont(f);
            }
        }
    }

private:
    void scrollToWidget(QWidget *w) {
        if (!w) return;
        scroll->ensureWidgetVisible(w, 0, 18);
    }
    void goToPlay() {
        scrollToWidget(playSection);
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("Los Santos — Browser Gaming");
    LosSantosWindow window;
    window.show();
    return app.exec();
}
