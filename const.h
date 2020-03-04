#ifndef CONST_H
#define CONST_H

#include<QString>

const QString FILE_DIR = "save_matrixs";

const QString FUNCTION[] = {
    "sin", "cos", "tan", "asin",
    "acos", "atan", "log"
};

enum FUNC_NUM {SIN = 0, COS, TAN,
               ASIN, ACOS, ATAN,
               LOG, NONE};

const double PRECISION = 1e-8;

const QString HELP_STR = "\n\n\nCopyright@Jxxxx\n"
                         "Email:870830732@qq.com\n\n\n\n"
                         "Help:\n"
                         "1.Input \"$reload$\" in \"Matrix name\" or press the reload button to reload\n"
                         "2.Input \"$delete$\" + \"the name of the matrix\""
                         " which you want to delete in \"Matrix name\" to delete it\n"
                         "3.The name of the Matrix can ONLY contain letters and numbers and \'_\'\n"
                         "  And the name can ONLY begin with letters\n"
                         "4.When you input a Matrix:\n"
                         "  use comma(\",\") to identify coloums\n"
                         "  use enter(\"\\n\") to identify rows\n\n"
                         "  example:\n"
                         "    Matrix name: MyFirst\n"
                         "    Matrix Data:\n"
                         "      1,0,sin(3.1415926535)\n"
                         "      33/(2*16.5)-1,1,0\n"
                         "      0,55-5*11,1\n\n"
                         "    And you will get:\n"
                         "    >>MyFirst:\n"
                         "            1        0        0\n"
                         "            0        1        0\n"
                         "            0        0        1\n\n"
                         "5.Input \"ANS\" in \"input\" of the \"Calculate\" Page to access the ans of the last time\n"
                         "6.Currently you can use those functions:\n"
                         "  sin, cos, tan, asin, scos, atan, log\n";

enum ERROR {CORRECT, ADD, SUB, MULTI, DIVIDE, POW, DET, SIZE};

#endif // CONST_H
