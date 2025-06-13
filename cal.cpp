#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>

class Calculator : public QWidget {
public:
    Calculator(QWidget *parent = nullptr) : QWidget(parent) {
        display = new QLineEdit(this);
        QGridLayout *layout = new QGridLayout(this);
        layout->addWidget(display, 0, 0, 1, 4);

        QStringList buttons = {
            "7", "8", "9", "/",
            "4", "5", "6", "*",
            "1", "2", "3", "-",
            "0", "C", "=", "+"
        };

        int row = 1, col = 0;
        for (const QString &text : buttons) {
            QPushButton *btn = new QPushButton(text, this);
            layout->addWidget(btn, row, col);
            connect(btn, &QPushButton::clicked, [=]() { onButtonClicked(text); });

            if (++col == 4) {
                col = 0;
                ++row;
            }
        }
    }

private:
    QLineEdit *display;
    QString currentInput;

    void onButtonClicked(const QString &text) {
        if (text == "C") {
            currentInput.clear();
        } else if (text == "=") {
            currentInput = evaluate(currentInput);
        } else {
            currentInput += text;
        }
        display->setText(currentInput);
    }

    QString evaluate(const QString &expr) {
        // Very basic evaluation (no error checking, no parentheses)
        double result = 0;
        QString op;
        QStringList tokens;
        QString num;

        for (QChar ch : expr) {
            if (ch.isDigit() || ch == '.') {
                num += ch;
            } else {
                if (!num.isEmpty()) {
                    tokens << num;
                    num.clear();
                }
                tokens << ch;
            }
        }
        if (!num.isEmpty()) tokens << num;

        if (tokens.size() < 3) return expr;

        result = tokens[0].toDouble();
        for (int i = 1; i < tokens.size(); i += 2) {
            QString oper = tokens[i];
            double next = tokens[i + 1].toDouble();
            if (oper == "+") result += next;
            else if (oper == "-") result -= next;
            else if (oper == "*") result *= next;
            else if (oper == "/") result /= next;
        }
        return QString::number(result);
    }
};
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Calculator calc;
    calc.setWindowTitle("Simple Qt Calculator");
    calc.resize(250, 200);
    calc.show();
    return app.exec();
}

