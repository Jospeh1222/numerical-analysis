#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Matematiksel sabitler
#define PI 3.14159265
#define e  2.71828

// İfade ağacı düğümü yapısı
typedef struct exprtree {
    char type;  // Düğüm tipi: sayı, işlem, fonksiyon vs.
    double value;  // Düğümdeki sayısal değer
    struct exprtree* left;  // Sol düğüm
    struct exprtree* right;  // Sağ düğüm
} exprtree;

// Parser yapısı: token dizisi ve pozisyonunu tutar
typedef struct parser_t {
    char* tokens;  // Token dizisi
    int ntokens;  // Token sayısı
    int pos;  // Mevcut pozisyon
} parser_t;

#define MAX_INPUT_SIZE 100

// Gregory-Newton interpolasyonu için yapı
typedef struct Gregory {
    int* x[MAX_INPUT_SIZE];  // X değerleri
    int* fx[MAX_INPUT_SIZE];  // Fonksiyon değerleri
    struct Gregory* next;  // Sonraki düğüm
} Gregory;

// Geçerli token karakterleri
#define VALID_TOKENS "+-*/^0123456789().sincotalgex_ "

// Fonksiyon prototipleri
char* tokenize(char*);
exprtree* parse(char*, double);
double calculate(exprtree*);
void free_exprtree(exprtree*);
exprtree* create_exprtree(char, double, exprtree*, exprtree*);
exprtree* parse_add_expression(parser_t*, double);
exprtree* parse_mult_expression(parser_t*, double);
exprtree* parse_atomic_expression(parser_t*, double);
exprtree* parse_number(parser_t*, double);
exprtree* parse_trig_function(parser_t*, double);
exprtree* parse_invtrig_function(parser_t*, double);
exprtree* parse_pow(parser_t*, double);
exprtree* parse_log(parser_t*, double);

// Diğer matematiksel yöntemlerin prototipleri
double Div(double, char*, double);
double Divileri(double, char*, double);
double Divgeri(double, char*, double);
double determinant(double**, int);
void EnBuyukKosegen(double**, double*, int);
void swapRows(double**, double*, int, int, int);
void Bisection();
void Regula_falsi();
void Newton_rapshon();
void Matrix_Inv();
void Gauss_Eliminasyon();
void Gauss_Seidel();
void Simpson();
void Sayisal_turev();
void Trapez(); 
void Gregory_Newton();
// Ana fonksiyon
int main(int argc, char* argv[]) {
    while (1) {
        // Menü seçeneklerini yazdır
        printf("\nGirmek istediginiz islemi secin\n");
        printf("Quit 0\n");
        printf("Bisection icin 1\n");
        printf("Regula-falsi icin 2\n");
        printf("Newton-Raphson icin 3\n");
        printf("NxNlik bir matrisin tersi icin 4\n");
        printf("Gauss eliminasyon icin 5\n");
        printf("Gauss seidal icin 6\n");
        printf("Sayisal turev icin 7\n");
        printf("Simpson icin 8\n");
        printf("Trapez icin 9\n");
        printf("Degisken donusumsuz Gregory Newton enterpolasyonu icin 10\n");

        int menu;
        printf("\nSeciminiz:");
        scanf("%d",&menu);
        switch (menu)
        {
        case 0:
             exit(1);
             break;
        case 1:
            Bisection();
            break;
        case 2:
            Regula_falsi();
            break;
        case 3:
            Newton_rapshon();
            break;
        case 4:
            Matrix_Inv();
            break;
        case 5:
            Gauss_Eliminasyon();
            break;
        case 6:
            Gauss_Seidel();
            break;
        case 7:
            Sayisal_turev();
            break;
        case 8:
            Simpson();
            break;
        case 9:
            Trapez();
            break;
        case 10:
            Gregory_Newton();
            break;
        }

    }
    return 0;
}

// Menü seçeneklerini yazdır
char* tokenize(char* in) {
    char* tokens = malloc(sizeof(char) * MAX_INPUT_SIZE);
    if (tokens == NULL) {
        fprintf(stderr, "Bellek tahsisi hatasi.\n");
        exit(EXIT_FAILURE);
    }

    int token_pos = 0,i;
    int in_len = strlen(in);
    for (i = 0; i < in_len; i++)
        if (strchr(VALID_TOKENS, in[i]))
            tokens[token_pos++] = in[i];

    tokens[token_pos] = '\0';
    return tokens;
}

// Token dizisini parse edip ifade ağacı oluşturur
exprtree* parse(char* tokens,double x) {
    parser_t* parser = malloc(sizeof(parser_t));
    if (parser == NULL) {
        fprintf(stderr, "Bellek tahsisi hatasi.\n");
        exit(EXIT_FAILURE);
    }

    parser->tokens = tokens;
    parser->ntokens = strlen(tokens);
    parser->pos = 0;

    exprtree* expression = parse_add_expression(parser,x);

    free(parser);
    return expression;
}


// İfade ağacını hesaplar
double calculate(exprtree* expr) {
    double rad = PI / 180;
    if (expr->type == 'n')
        return expr->value;
    else if (expr->type == 's')
        return sin(calculate(expr->left) * rad);
    else if (expr->type == 'c')
        return cos(calculate(expr->left) * rad);
    else if (expr->type == 't')
        return tan(calculate(expr->left) * rad);
    else if (expr->type == 'o')
        return 1 / tan(calculate(expr->left) * rad);
    else if (expr->type == 'S')
        return asin(calculate(expr->left))* (1 / rad);
    else if (expr->type == 'C')
        return acos(calculate(expr->left))* (1 / rad);
    else if (expr->type == 'T')
        return atan(calculate(expr->left))* (1 / rad);
    else if (expr->type == 'O')
        return 90 - atan(calculate(expr->left))* (1 / rad);
    else if (expr->type == '+')
        return calculate(expr->left) + calculate(expr->right);
    else if (expr->type == '-')
        return calculate(expr->left) - calculate(expr->right);
    else if (expr->type == '*')
        return calculate(expr->left) * calculate(expr->right);
    else if (expr->type == '/')
        return calculate(expr->left) / calculate(expr->right);
    else if (expr->type == '^')
        return pow(calculate(expr->left), calculate(expr->right));
    else if (expr->type == 'l')
        return (log(calculate(expr->right)))/(log(calculate(expr->left)));
    return 0;
}

// İfade ağacını serbest bırakır
void free_exprtree(exprtree* expr) {
    if (expr) {
        if (expr->left)
            free_exprtree(expr->left);
        if (expr->right)
            free_exprtree(expr->right);
        free(expr);
    }
}

// Yeni bir ifade ağacı düğümü oluşturur
exprtree* create_exprtree(char type, double value, exprtree* left, exprtree* right) {
    exprtree* expr = malloc(sizeof(exprtree));
    if (expr == NULL) {
        fprintf(stderr, "Bellek tahsisi hatasi.\n");
        exit(EXIT_FAILURE);
    }

    expr->type = type;
    expr->value = value;
    expr->left = left;
    expr->right = right;

    return expr;
}

// Toplama ve çıkarma ifadelerini parse eder
exprtree* parse_add_expression(parser_t* parser,double x) {
    exprtree* expr = parse_mult_expression(parser,x);

    while (parser->pos < parser->ntokens && (parser->tokens[parser->pos] == '+' || parser->tokens[parser->pos] == '-')) {
        char type = parser->tokens[parser->pos];
        parser->pos++;
        exprtree* right_expr = parse_mult_expression(parser,x);
        expr = create_exprtree(type, 0, expr, right_expr);
    }
    return expr;
}

// Çarpma ve bölme ifadelerini parse eder
exprtree* parse_mult_expression(parser_t* parser,double x) {
    exprtree* expr = parse_pow(parser,x);

    while (parser->pos < parser->ntokens && (parser->tokens[parser->pos] == '*' || parser->tokens[parser->pos] == '/')) {
        char type = parser->tokens[parser->pos];
        parser->pos++;
        exprtree* right_expr = parse_pow(parser,x);
        expr = create_exprtree(type, 0, expr, right_expr); 
    }
    return expr;
}

// Üs alma ifadelerini parse eder
exprtree* parse_pow(parser_t* parser,double x) {

     exprtree* expr = parse_atomic_expression(parser,x);

    while (parser->pos < parser->ntokens && (parser->tokens[parser->pos] == '^')) {
        char type = parser->tokens[parser->pos];
        parser->pos++;
        exprtree* right_expr = parse_atomic_expression(parser,x);
        expr = create_exprtree(type, 0, expr, right_expr); 
    }

    return expr;
}

// Atomik ifadeleri parse eder (sayilar, parantezler, fonksiyonlar)
exprtree* parse_atomic_expression(parser_t* parser,double x) {
    exprtree* expr;

    if (parser->tokens[parser->pos] == '(') {
        parser->pos++;
        expr = parse_add_expression(parser,x);
        if (parser->tokens[parser->pos] == ')')
            parser->pos++;
        else {
            fprintf(stderr, "Gecersiz giris\n");
            exit(EXIT_FAILURE);
        }
    } else if (parser->tokens[parser->pos] == 's' || parser->tokens[parser->pos] == 'c' || parser->tokens[parser->pos] == 't' ) {
        expr = parse_trig_function(parser,x);
    } else if (parser->tokens[parser->pos] == 'a') {
        expr = parse_invtrig_function(parser,x);
    } else if (parser->tokens[parser->pos] == 'l') {
        expr = parse_log(parser,x);
    } else {
        expr = parse_number(parser,x);
    }
    return expr;
}

// Trigonometrik fonksiyonları parse eder
exprtree* parse_trig_function(parser_t* parser,double x) {
    char type = parser->tokens[parser->pos];
    if (type != 'c' && type != 's' && type != 't') {
        fprintf(stderr, "Gecersiz giris\n");
        exit(EXIT_FAILURE);
    }

    if (type == 's' && parser->tokens[parser->pos + 1] == 'i' && parser->tokens[parser->pos + 2] == 'n')
        type = 's';
    else if (type == 'c' && parser->tokens[parser->pos + 1] == 'o' && parser->tokens[parser->pos + 2] == 's')
        type = 'c';
    else if (type == 't' && parser->tokens[parser->pos + 1] == 'a' && parser->tokens[parser->pos + 2] == 'n')
        type = 't';
    else if (type == 'c' && parser->tokens[parser->pos + 1] == 'o' && parser->tokens[parser->pos + 2] == 't')
        type = 'o';
    parser->pos += 3;

    if (parser->tokens[parser->pos] == '(') {
        parser->pos++;
        exprtree* arg_expr = parse_add_expression(parser,x); 
        if (parser->tokens[parser->pos] == ')')
            parser->pos++;
        else {
            fprintf(stderr, "Gecersiz giris\n");
            exit(EXIT_FAILURE);
        }
        return create_exprtree(type, 0, arg_expr, NULL); 
    } else {
        return create_exprtree(type, 0, NULL, NULL); 
    }
}

// Ters trigonometrik fonksiyonları parse eder
exprtree* parse_invtrig_function(parser_t* parser,double x) {
    parser->pos += 2;
    char type = parser->tokens[parser->pos];
    if (type != 'c' && type != 's' && type != 't') {
        fprintf(stderr, "Gecersiz giris\n");
        exit(EXIT_FAILURE);
    }
    if (type == 's' && parser->tokens[parser->pos + 1] == 'i' && parser->tokens[parser->pos + 2] == 'n')
        type = 'S';
    else if (type == 'c' && parser->tokens[parser->pos + 1] == 'o' && parser->tokens[parser->pos + 2] == 's')
        type = 'C';
    else if (type == 't' && parser->tokens[parser->pos + 1] == 'a' && parser->tokens[parser->pos + 2] == 'n')
        type = 'T';
    else if (type == 'c' && parser->tokens[parser->pos + 1] == 'o' && parser->tokens[parser->pos + 2] == 't')
        type = 'O';
    parser->pos += 3;

    if (parser->tokens[parser->pos] == '(') {
        parser->pos++;
        exprtree* arg_expr = parse_add_expression(parser,x); 
        if (parser->tokens[parser->pos] == ')')
            parser->pos++;
        else {
            fprintf(stderr, "Gecersiz giris\n");
            exit(EXIT_FAILURE);
        }
        return create_exprtree(type, 0, arg_expr, NULL); 
    } else {
        return create_exprtree(type, 0, NULL, NULL); 
    }
}

// Logaritma ifadelerini parse eder
exprtree* parse_log(parser_t* parser,double x)
{
    char type = parser->tokens[parser->pos];
    if (type != 'l' ) {
        fprintf(stderr, "Gecersiz giris\n");
        exit(EXIT_FAILURE);
    }
    if (type == 'l' && parser->tokens[parser->pos + 1] == 'o' && parser->tokens[parser->pos + 2] == 'g')
        type = 'l';
    parser->pos += 3;

    if (parser->tokens[parser->pos] == '_') {
        parser->pos++;
        exprtree* arg_expr = parse_add_expression(parser,x); 
        if (parser->tokens[parser->pos] == '('){
            parser->pos++; 
        }else {
            fprintf(stderr, "Gecersiz giris\n");
            exit(EXIT_FAILURE);
        }
        exprtree* arg_expr1 = parse_add_expression(parser,x);
        if(parser->tokens[parser->pos] == ')')
        {
           parser->pos++;  
        }
        else {
            fprintf(stderr, "Gecersiz giris\n");
            exit(EXIT_FAILURE);
        }
        return create_exprtree(type, 0, arg_expr, arg_expr1 ); 
    } else {
        return create_exprtree(type, 0, NULL, NULL); 
    }

}

// Sayıları parse eder
exprtree* parse_number(parser_t* parser,double x) {
    char number[MAX_INPUT_SIZE];
    int numberlen = 0;
    double  value;

    while (strchr("0123456789.xe", parser->tokens[parser->pos]) && numberlen < MAX_INPUT_SIZE && parser->pos < parser->ntokens) {
        number[numberlen++] = parser->tokens[parser->pos];
        parser->pos++;
    }
    number[numberlen] = '\0';

    int i = 0;
    int tmp = 0;
    while (number[i] != '\0' && tmp != 1) {
        if (number[i] == 'x') 
        {
            value = x;
            tmp = 1;
        }
        i++;
    }
    i = 0;
    while (number[i] != '\0' && tmp != 1) {
        if (number[i] == 'e') 
        {
            value = e;
            tmp = 1;
        }
        i++;
    }
    if (tmp == 0)
        value = atof(number);

    return create_exprtree('n', value, NULL, NULL);
}

 ///////////////////////////////////////////////////////////////////
// Merkezi fark ile türev hesaplama
double Div(double x, char* tokens, double h)
{
    // x-h noktasında fonksiyon değerini hesapla
    exprtree* expression1 = parse(tokens, x - h);
    double kucuk = calculate(expression1);
    
    // x+h noktasında fonksiyon değerini hesapla
    exprtree* expression2 = parse(tokens, x + h);
    double buyuk = calculate(expression2);

    // Merkezi fark yöntemi ile türevi hesapla ve döndür
    return (buyuk - kucuk) / ((x + h) - (x - h));
}

// İleri fark yöntemi ile türev hesaplama 
double Divileri(double x, char* tokens, double h)
{
    // x noktasında fonksiyon değerini hesapla
    exprtree* expression1 = parse(tokens, x);
    double kucuk = calculate(expression1);
    
    // x+h noktasında fonksiyon değerini hesapla
    exprtree* expression2 = parse(tokens, x + h);
    double buyuk = calculate(expression2);

    // İleri fark yöntemi ile türevi hesapla ve döndür
    return (buyuk - kucuk) / h;
}

// Geri fark yöntemi ile türev hesaplama
double Divgeri(double x, char* tokens, double h)
{
    // x-h noktasında fonksiyon değerini hesapla
    exprtree* expression1 = parse(tokens, x - h);
    double kucuk = calculate(expression1);
    
    // x noktasında fonksiyon değerini hesapla
    exprtree* expression2 = parse(tokens, x);
    double buyuk = calculate(expression2);

    // Geri fark yöntemi ile türevi hesapla ve döndür
    return (buyuk - kucuk) / h;
}

// Matris belleğini serbest bırakma fonksiyonu
void freeMatrix(double **matrix, int N) {
    int i;
    for (i=0;i<N;i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Matrisi yazdırma fonksiyonu
void printMatrix(double **matrix, int N) 
{
    int i,j;
    for (i=0;i<N;i++) 
    {
        for (j=0;j<N;j++) 
        {
            printf("%lf ", matrix[i][j]);
        }
        printf("\n");
    }
}


// Matrisin determinantını hesaplayan fonksiyon
double determinant(double **matrix, int N) 
{
    double det = 1.0;
    int i,k,j;
    // Satır satır ilerleyerek matrisin üst üçgensel hale getirilmesi
    for (i=0;i<N;i++) 
    {
        // Eğer köşegen eleman sıfırsa, sıfır olmayan bir elemanla yer değiştir
        if (matrix[i][i] == 0.0) 
        {
            int swapFound = 0;
            for (k=i+1;k<N && !swapFound;k++) 
            {
                if (matrix[k][i] != 0.0) 
                {
                    // Satırları değiştir ve determinantı negatifle
                    double *temp = matrix[i];
                    matrix[i] = matrix[k];
                    matrix[k] = temp;
                    det *= -1;
                    swapFound = 1;
                }
            }
            // Eğer sıfırdan farklı bir eleman bulunamazsa, determinant sıfırdır
            if (!swapFound) 
            {
                return 0.0; // Matris tekildir
            }
        }

        // İleri eleme yöntemi ile matrisi üst üçgensel hale getir
        for (k=i+1;k<N;k++) 
        {
            double factor = matrix[k][i] / matrix[i][i];
            for (j=i;j<N;j++) 
            {
                matrix[k][j] -= factor * matrix[i][j];
            }
        }
        // Determinant değerini güncelle
        det *= matrix[i][i];
    }
    return det;
}

// Satırları değiştiren fonksiyon
void swapRows(double **matrix, double *result, int row1, int row2, int n) {
    // İki satırın yerini değiştir
    double *tempRow = matrix[row1];
    matrix[row1] = matrix[row2];
    matrix[row2] = tempRow;
    
    // Sonuç vektöründe de değişiklik yap
    double tempResult = result[row1];
    result[row1] = result[row2];
    result[row2] = tempResult;
}

// Matrisin köşegeninde en büyük çarpanı elde eden ve gerekirse satır değişimi yapan fonksiyon
void EnBuyukKosegen(double **matrix, double *results, int n) {
    int i,j;
    for (i=0;i<n;i++) {
        // Maksimum elemanın indisini ve değerini sakla
        int maxRow = i;
        double maxElement = fabs(matrix[i][i]);

        for (j=i+1;j<n;j++) {
            // Şu ana kadar bulunan en büyük elemandan daha büyük bir eleman varsa,
            // bu elemanın değerini ve indisini güncelle
            if (fabs(matrix[j][i]) > maxElement) {
                maxElement = fabs(matrix[j][i]);
                maxRow = j;
            }
        }

        // Eğer bulunan en büyük elemanın indisinde değişiklik yapıldıysa,
        // bu iki satırı yer değiştir
        if (maxRow != i) {
            swapRows(matrix, results, i, maxRow, n);
        }
    }
}

// Bisection yöntemiyle kök bulan fonksiyon
void Bisection()
{
    double x;

    //Kullanıcıdan ifade istenir ve tokenlere ayrılır
    char in[MAX_INPUT_SIZE];
    printf("Matematiksel ifadeyi girin:");
    scanf("\n%[^\n]", in); 
    char* tokens = tokenize(in);      

        //x var mı diye arar varsa x değerini sorar yoksa fonksiyondan çıkar
        int i=0,tmp=0,j=1;
        while(tokens[i]!='\0'&&tmp!=1)
        {
            if(tokens[i]=='x')
            {
                tmp=1;
            }
            i++;
        }
        if(tmp==0)
        {
            printf("Girdiginiz ifade x icermemektedir");
            exit(0);
        }


    //kullanıcıddan aralığın başlangıç ve bitiş değerleri alınır
    double left;
    printf("Araligin baslangicini giriniz:");
    scanf("%lf",&left);
    x=left;
    exprtree* expression1 = parse(tokens,x);
    double kucuk = calculate(expression1);
    
    double right;
    printf("Araligin bitisini giriniz:");
    scanf("%lf",&right);
    x=right;
    exprtree* expression2 = parse(tokens,x); 
    double buyuk = calculate(expression2);

    //Eğer aralıkta kök yoksa çık
    double hataPay,yeni,yenideg;
    if(kucuk*buyuk>0)
    {
        printf("Girdiğiniz aralikta kok bulunmamaktadir");
        exit(0);
    }else//kök varsa 
    {
        printf("Hata payini giriniz:");
        scanf("%lf",&hataPay);
        while((right-left)/pow(2,j)>hataPay)// Hata payını karşılayıncaya kadar döngü
        {
            yeni = (right + left) / 2; // Yeni değer hesaplanır
            exprtree* expression3 = parse(tokens, yeni); // Yeni değer ile ifade hesaplanır
            yenideg = calculate(expression3); // Hesaplanan değer yenideg değişkenine atanır
            if(yenideg*buyuk<0)// Eğer yeni değer ile bitiş değeri aynı işaretliyse
            {
                left = yeni; // Yeni değer sol sınır yapılır
                kucuk = yenideg; // Yeni değer kucuk değişkenine atanır
                printf("\n%d. iterasyon degeri:%lf",j,yeni);
            }else if(yenideg*kucuk<0)// Eğer yeni değer ile başlangıç değeri aynı işaretliyse
            {
                right = yeni; // Yeni değer sağ sınır yapılır
                buyuk = yenideg; // Yeni değer buyuk değişkenine atanır
                printf("\n%d. iterasyon degeri:%lf",j,yeni);
            }
             j++;
        }
        yeni=(right+left)/2;// Son yaklaşık değer hesaplanır
        printf("\n%d. iterasyon yaklasik kok degeri:%lf\n",j,yeni);// Yaklaşık kök değeri ekrana yazdırılır
    }

    free_exprtree(expression1);
    free_exprtree(expression2);
    
    free(tokens);
 }


// Regula Falsi yöntemiyle kök bulan fonksiyon
 void Regula_falsi()
 {
    
    //Kullanıcıdan ifade istenir ve tokenlere ayrılır
    double x;
    char in[MAX_INPUT_SIZE];
    printf("Matematiksel ifadeyi girin:");
    scanf("\n%[^\n]", in); 
        //x var mı diye arar varsa x değerini sorar yoksa fonksiyondan çıkar
        char* tokens = tokenize(in);      
        int i=0,tmp=0,j=1;
        while(tokens[i]!='\0'&&tmp!=1)
        {
            if(tokens[i]=='x')
            {
                tmp=1;
            }
            i++;
        }
        if(tmp==0)
        {
            printf("girdiginiz ifade x icermemektedir\n");
            exit(0);
        }

    //kullanıcıddan aralığın başlangıç ve bitiş değerleri alınır
    double left;
    printf("Araligin baslangicini giriniz:");
    scanf("%lf",&left);
    x=left;
    exprtree* expression1 = parse(tokens,x);
    double leftdeg = calculate(expression1);
    
    double right;
    printf("Araligin bitisini giriniz:");
    scanf("%lf",&right);
    x=right;
    exprtree* expression2 = parse(tokens,x); 
    double rightdeg = calculate(expression2);

    //Eğer aralıkta kök yoksa çık
    double hataPay,newPoint;
    if(rightdeg*leftdeg>0)
    {
        printf("\nBu kapali aralikta kok bulunmamaktadir\n");
        exit(0);
    }else
    {
    printf("Hata payini giriniz:");
    scanf("%lf",&hataPay);
    newPoint=((left*rightdeg)-(right*leftdeg))/(rightdeg-leftdeg);
       while((right-left)/pow(2,j)>hataPay||(left-right)/pow(2,j)>hataPay)// Hata payını karşılayıncaya kadar döngü
       {
        printf("\n%d. iterasayon:%lf",j,newPoint);
       
        exprtree* expression3 = parse(tokens, newPoint); // Yeni değer ile ifade hesaplanır
        double newDeger = calculate(expression3); // Hesaplanan değer newDeger değişkenine atanır
           
        if(newDeger * leftdeg < 0) // Eğer yeni değer ile başlangıç değeri aynı işaretliyse
            { 
                right = newPoint; // Yeni değer sağ sınır yapılır
                rightdeg = newDeger; // Yeni değer rightdeg değişkenine atanır
            }
            else if(newDeger * rightdeg < 0) // Eğer yeni değer ile bitiş değeri aynı işaretliyse
            {
                left = newPoint; // Yeni değer sol sınır yapılır
                leftdeg = newDeger; // Yeni değer leftdeg değişkenine atanır
            }
         newPoint=((left*rightdeg)-(right*leftdeg))/(rightdeg-leftdeg);
        j++;
        newDeger=0;
       } 
    printf("\nYaklasik deger:%lf\n",newPoint);
    }
    free_exprtree(expression1);
    free_exprtree(expression2);
    free(tokens);

 }


// Newton-Raphson yöntemiyle kök bulan fonksiyon
 void Newton_rapshon()
 {
    double x;
    double h=0.0001;
    char in[MAX_INPUT_SIZE];
    
    // Kullanıcıdan matematiksel ifadeyi alır ve tokenlere ayırır
    printf("Matematiksel ifadeyi girin:");
    scanf("\n%[^\n]", in); 
        
        char* tokens = tokenize(in);      
        int i=0,tmp=0,j=1;
        while(tokens[i]!='\0'&&tmp!=1)
        {
            if(tokens[i]=='x')
            {
                tmp=1;
            }
            i++;
        }
        if(tmp==0)
        {
            printf("girdiginiz ifade x icermemektedir\n");
            exit(0);
        }

    //Kullanıcıdan aralık değerleri alınır
    double left;
    printf("Araligin baslangicini giriniz:");
    scanf("%lf",&left);
    x=left;
    exprtree* expression1 = parse(tokens,x);
    double leftdeg = calculate(expression1);
    
    double right;
    printf("Araligin bitisini giriniz:");
    scanf("%lf",&right);
    x=right;
    exprtree* expression2 = parse(tokens,x); 
    double rightdeg = calculate(expression2);

    if(rightdeg*leftdeg>0)
    {
        printf("\nBu kapali aralikta kok bulunmamaktadir\n");
        exit(0);
    }

    double baslangic = left; // Kök bulma işlemine başlanacak nokta
    double baslangicdeg = leftdeg; // Başlangıç noktasının değeri
    double turev1 = Div(baslangic, tokens, h); // Başlangıç noktasının türev değeri

    double x1,x1Deger;//yeni nokta
    //yeni nokta heasplanır
    x1 = baslangic-(baslangicdeg/turev1);

    double hataPay,y;
    printf("Hata payini giriniz(10 uzeri):");
    scanf("%lf",&y);
    hataPay=pow(10,y);

    //hata payını karşılayıncaya kadar döngü
    while(fabs(x1-baslangic)>hataPay)
    {

        printf("\n%d. iterasyon :%lf",j,x1);
        baslangic = x1; // Yeni nokta eski nokta yapılır
        turev1 = Div(x1, tokens, h); // Yeni noktanın türevi hesaplanır
        exprtree* expression3 = parse(tokens, x1); 
        x1Deger = calculate(expression3); 
        x1=x1-(x1Deger/turev1);
        j++;

    }
    printf("\nyaklasik kok:%lf",x1);

    free_exprtree(expression1);
    free_exprtree(expression2);
    free(tokens);
 }


//matrix tersini bulan fonksiyon
void Matrix_Inv()
{
    int N,i,j,k;
    printf("NxN bir matrisin N degerini giriniz: ");
    scanf("%d", &N);

    // Matris ve ters matris için bellek ayırır
    double **matrix = (double **)calloc(N, sizeof(double *));
    double **invMatrix = (double **)calloc(N, sizeof(double *));
    for (i=0;i<N;i++) {
        matrix[i] = (double *)calloc(N, sizeof(double));
        invMatrix[i] = (double *)calloc(N, sizeof(double));
        invMatrix[i][i] = 1.0; // Birim matrisi başlatır
    }

    for (i=0;i<N;i++) 
    {
        for (j=0;j<N;j++) 
        {
            printf("Matrisin a(%d,%d) elemanini giriniz: ",i+1,j+1);
            scanf("%lf", &matrix[i][j]);
        }
    }

    // Determinantı kontrol eder
    double **matrixCopy = (double **)calloc(N, sizeof(double *));
    for (i=0;i<N;i++) 
    {
        matrixCopy[i] = (double *)calloc(N, sizeof(double));
        for (j=0;j<N;j++) 
        {
            matrixCopy[i][j] = matrix[i][j];
        }
    }
    double det = determinant(matrixCopy, N);
    if (det == 0.0) {
        printf("Matris tekildir ve tersi alinamaz.\n");
        freeMatrix(matrix, N);
        freeMatrix(invMatrix, N);
        freeMatrix(matrixCopy, N);
        exit(0);
    }
    freeMatrix(matrixCopy, N);


    // Gauss-Jordan Eliminasyonu
    for (i=0;i<N;i++) 
    {
        //pivot eleman kontrolü
        if (matrix[i][i] == 0) 
        {
            int swapFound = 0;
            // Satır değişimi yapılacak uygun elemanın aranması
            for (k=i+1;k<N && !swapFound;k++) 
            {
                if (matrix[k][i] != 0) 
                {
                    // Satırların değiştirilmesi
                    double *temp = matrix[i];
                    matrix[i] = matrix[k];
                    matrix[k] = temp;
                    temp = invMatrix[i];
                    invMatrix[i] = invMatrix[k];
                    invMatrix[k] = temp;
                    swapFound = 1;
                }
            }
            // Satır değişimi yapılamazsa
            if (!swapFound) 
            {
                printf("Matris tekildir ve tersi yoktur.\n");
                // Bellekten serbest bırakma
                freeMatrix(matrix, N);
                freeMatrix(invMatrix, N);
                exit(0);
            }
        }

        // Diagonal elemanın alınması
        double diagElement = matrix[i][i];
        // Diagonal elemanın 1'e normalize edilmesi
        for (j=0;j<N;j++) 
        {
            matrix[i][j] /= diagElement;
            invMatrix[i][j] /= diagElement;
        }

        // Diagonal eleman dışındaki elemanların sıfırlanması
        for (k=0;k<N;k++) 
        {
            if (k!=i) 
            {
                double factor = matrix[k][i];
                for (j=0;j<N;j++) 
                {
                    matrix[k][j]-=factor*matrix[i][j];
                    invMatrix[k][j]-=factor*invMatrix[i][j];
                }
            }
        }
    }

    // Ters matrisi yazdır
    printf("\nTers Matris:\n");
    printMatrix(invMatrix, N);

    // Belleği serbest bırak
    freeMatrix(matrix, N);
    freeMatrix(invMatrix, N);

}


void Gauss_Eliminasyon()
{
    printf("Degisken sayisini giriniz: ");
    int n,i,j,k;
    scanf("%d", &n);

    double **matrix = (double **)calloc(n, sizeof(double *));
    for (i=0;i<n;i++) {
        matrix[i] = (double *)calloc(n, sizeof(double)); // Katsayı matrisi
    }

    double *results = (double *)calloc(n, sizeof(double)); // Sonuç matrisi


    //matrixin prototipinin yazdırılması ve kullanıcıdan değerlerin alınması
    char deg = 'a';
    for (i=0;i<n;i++) {
        for (j=0;j<n;j++) 
        {
            if (j == n-1) 
            {
                printf("%c%d ", deg+j,i+1);
            } else 
            {
                printf("%c%d + ",deg+j,i+1);
            }
        }
        printf("= %c%d\n",deg+j,i+1);
    }
    printf("Degiskenleri giriniz:\n");
    for (i=0;i<n;i++) {
        for (j=0;j<n;j++) {
            printf("%c%d: ", deg+j,i+1);
            scanf("%lf", &matrix[i][j]);
        }
        printf("= %c%d: ", deg+j,i+1);
        scanf("%lf", &results[i]);
        printf("\n");
    }

    // Matrisin tekillik kontrolü
    double det = determinant(matrix, n);
    if (det == 0) {
        printf("Matris tekildir ve tersi alinamaz.\n");
        freeMatrix(matrix, n);
        free(results);
        exit(0);
    }

    // Gauss-Jordan eliminasyonu
    for (i=0;i<n;i++) 
    {
        // Pivot seçimi ve normalize etme
        double pivot = matrix[i][i];
        if (pivot == 0) 
        {
            // Satır değiştirme
            int swapFound = 0;
            for (k=i+1;k<n && !swapFound;k++) 
            {
                if (matrix[k][i] != 0) 
                {
                    double *temp = matrix[i];
                    matrix[i] = matrix[k];
                    matrix[k] = temp;
                    double tempResult = results[i];
                    results[i] = results[k];
                    results[k] = tempResult;
                    swapFound = 1;
                }
            }
            pivot = matrix[i][i];
            if (!swapFound || pivot == 0) 
            {
                printf("Matris tekildir ve tersi alinamaz.\n");
                freeMatrix(matrix, n);
                free(results);
                exit(0);
            }
        }

        for (j=0;j<n;j++) 
        {
            matrix[i][j]/=pivot;
        }
        results[i]/=pivot;

        // Diğer satırlardan pivot sütunu çıkarma
        for (k=0;k<n;k++) 
        {
            if (k!=i) {
                double factor=matrix[k][i];
                for (j=0;j<n;j++) {
                    matrix[k][j]-=factor*matrix[i][j];
                }
                results[k]-=factor*results[i];
            }
        }
    }

    printf("\nSonuc:\n");
    for (i=0;i<n;i++) 
    {
        printf("%c = %lf\n",'a'+i, results[i]);
    }

    // Belleği serbest bırakma
    freeMatrix(matrix, n);
    free(results);

}


void Gauss_Seidel()
{
    printf("Degisken sayisini giriniz: ");
    int n, i, j, k;
    scanf("%d",&n);

    double **matrix = (double **)calloc(n, sizeof(double *));
    for (i = 0; i < n; i++) {
        matrix[i] = (double *)calloc(n, sizeof(double)); // Katsayı matrisi
    }

    double *sonuc = (double *)calloc(n, sizeof(double)); // Sonuç matrisi


    //matrixin prototipinin yazdırılması ve kullanıcıdan değerlerin alınması
    char deg='a';
    for (i=0;i<n;i++) 
    {
        for (j=0;j<n;j++) 
        {
            if (j == n-1) 
            {
                printf("%c%d ", deg+j,i+1);
            } else 
            {
                printf("%c%d + ",deg+j,i+1);
            }
        }
        printf("= %c%d\n",deg+j,i+1);
    }
    printf("Degiskenleri giriniz:\n");
    for (i=0;i<n;i++) 
    {
        for (j=0;j<n;j++) 
        {
            printf("%c%d: ", deg+j,i+1);
            scanf("%lf", &matrix[i][j]);
        }
        printf("%c%d: ", deg+j,i+1);
        scanf("%lf", &sonuc[i]);
        printf("\n");
    }

    //determinant kontrol etme 0 ise çıkma
    double **matrixCopy = (double**)calloc(n, sizeof(double *));
    for (i=0;i<n;i++) 
    {
        matrixCopy[i] = (double*)calloc(n, sizeof(double));
        for (j=0;j<n;j++) 
        {
            matrixCopy[i][j]=matrix[i][j];
        }
    }
    double det=determinant(matrixCopy, n);
    if (det==0.0) 
    {
        printf("Matris tekildir ve tersi alinamaz.\n");
        freeMatrix(matrix, n);
        freeMatrix(matrixCopy, n);
        exit(0);
    }
    freeMatrix(matrixCopy, n);
    EnBuyukKosegen(matrix,sonuc,n);

    // Başlangıç değerlerinin alınması ve hata payının girilmesi
    double *basDeg = (double *)calloc(n, sizeof(double));
    printf("Baslangic degerlerini giriniz:\n");
    for (i=0;i<n;i++) {
        printf("%c[%d]:",'a'+i,i+1);
        scanf("%lf", &basDeg[i]);
    }

    printf("Hata payini giriniz: ");
    double tolerance;
    scanf("%lf", &tolerance);

    // Önceki iterasyondaki değerlerin saklanması
    double *oncekiX = (double *)calloc(n, sizeof(double));
    double maxDifference;
    for (i=0;i<n;i++) 
    {
        oncekiX[i] = basDeg[i];
    }

    int iterations = 0;
    do 
    {
        // Gauss-Seidel iterasyonunun yapılması
        for (i=0;i<n;i++) 
        {
            double sum = sonuc[i];
            for (j=0;j<n;j++) 
            {
                if (i!=j) 
                {
                    sum-=matrix[i][j]*basDeg[j];
                }
            }
            basDeg[i]=sum/matrix[i][i];
        }

        // Yakınsama kontolü
        maxDifference=0;
        for (i=0;i<n;i++) 
        {
            double difference=fabs(basDeg[i]-oncekiX[i]);
            if (difference > maxDifference) 
            {
                maxDifference=difference;
            }
            oncekiX[i]=basDeg[i];  // önceki iterasyonun güncellenmesi
        }
        iterations++;
    } while (maxDifference>tolerance);

    printf("Gauss-Seidel iterasyonu %d iterasyonda tamamlandi.\n", iterations);

    for (i=0;i<n;i++) 
    {
        printf("%c[%d] = %lf\n",'a'+i,i+1,basDeg[i]);
    }
    free(matrix);
    free(sonuc);
    free(basDeg);


}


void Sayisal_turev()
 {
    //menü
    printf("\nIleri farklar icin:1");
    printf("\nGeri farklar icin:2");
    printf("\nMerkezi turev icin:3\n");

        int menu;
        double h , ileri ;
        scanf("%d",&menu);

    //kullanıcıdan ifadeyi alma ve x degerini alma
    double x;
    char in[MAX_INPUT_SIZE];
    printf("Matematiksel ifadeyi girin:");
        scanf("\n%[^\n]", in); 
        
        char* tokens = tokenize(in);      
        int i=0,tmp=0,j=1;
        while(tokens[i]!='\0'&&tmp!=1)
        {
            if(tokens[i]=='x')
            {
                tmp=1;
            }
            i++;
        }
        if(tmp==0)
        {
            printf("girdiginiz ifade x icermemektedir\n");
            exit(0);
        }
        printf("\nx degerini giriniz:");
        scanf("%lf",&x);

        switch(menu)
        {
            case 1:
                printf("h degerini giriniz:");
                scanf("%lf",&h);
                ileri=Divileri( x ,tokens,h);//ileri türevin fonksiyon yardımıyla hesaplanması
                printf("Ileri farklar icin turev:%lf\n",ileri);
                break;

            case 2:
                printf("h degerini giriniz:");
                scanf("%lf",&h);
                ileri=Divgeri( x ,tokens,h);//geri türevin fonksiyon yardımıyla hesaplanması
                printf("Geri farklar icin turev:%lf\n",ileri);
                break;

            case 3:
                printf("h degerini giriniz:");
                scanf("%lf",&h);
                ileri=Div( x ,tokens,h);//merkezi türevin fonksiyon yardımıyla hesaplanması
                printf("Merkezi farklar icin turev:%lf\n",ileri);
                break;
        }

 }

// Simpson 1/3 veya 3/8 kuralına göre integral hesaplama
 void Simpson()
 {

    //menü
    printf("\n1/3 simpson icin:1\n");
    printf("3/8 simpson icin:2\n");
    int menu;
    scanf("%d",&menu);

    //Kullanıcıdan ifadeyi alma 
    double x;
    char in[MAX_INPUT_SIZE];
    printf("Matematiksel ifadeyi girin:");
        scanf("\n%[^\n]", in); 
        
        char* tokens = tokenize(in);      
        int i=0,tmp=0,j=1;
        while(tokens[i]!='\0'&&tmp!=1)
        {
            if(tokens[i]=='x')
            {
                tmp=1;
            }
            i++;
        }
        if(tmp==0)
        {
            printf("girdiginiz ifade x icermemektedir\n");
            exit(0);
        }

        //kullanıcıdan integral için aralık alma
         double left;
         printf("Araligin baslangicini giriniz:");
         scanf("%lf",&left);
         x=left;
         exprtree* expression1 = parse(tokens,x);
         double leftdeg = calculate(expression1);
    
         double right;
         printf("Araligin bitisini giriniz:");
         scanf("%lf",&right);
         x=right;
         exprtree* expression2 = parse(tokens,x); 
         double rightdeg = calculate(expression2);
        
        double h;

        switch(menu)
        {
            case 1:
            printf("\nn degerini giriniz:");
            int n;
            scanf("%d",&n);
            double toplam=0,Tek,Cift;
            double h=(right-left)/n;// Adım boyutunu hesapla
            x=left+h;// x'i başlangıç noktasıyla güncelle
            for(i=1;i<n;i+=2)// Tek terimler için döngü
            {
                exprtree* expression3 = parse(tokens,x); 
                Tek = calculate(expression3);
                toplam=toplam+4*Tek; // Toplama 4 ile çarpılmış tek terimi ekle
                free_exprtree(expression3);
                x=x+2*h; // x değerini 2 adım arttır
            }
            x=left+2*h; // x'i başlangıç noktasından 2 adım uzaklaştır
            for(i=2;i<n;i+=2) // Çift terimler için döngü
            {
                exprtree* expression3 = parse(tokens,x); 
                Cift = calculate(expression3);
                toplam=toplam+2*Cift; // Toplama 2 ile çarpılmış çift terimi ekle
                free_exprtree(expression3);
                x=x+2*h;
            }
            toplam=h/3*(leftdeg+rightdeg+toplam); // Simpson 1/3 kuralına göre integrali hesapla
            toplam=fabs(toplam);
            printf("\nSimpson 1/3 kuralina gore integral degeri:%lf",toplam);
            break;

            case 2:
            n=3;
            toplam=0;
            h=(right-left)/n;// Adım boyutunu hesapla
            x=left;
            for(i=1;i<n;i++) // İlk ve son terim dışındaki terimler için döngü
            {
                x=x+h;
                exprtree* expression3 = parse(tokens,x); 
                Tek = calculate(expression3);
                toplam=toplam+3*Tek; // Toplama 3 ile çarpılmış terimi ekle
                free_exprtree(expression3);
            }
            toplam=((right-left)/8)*(leftdeg+rightdeg+toplam); // Simpson 3/8 kuralına göre integrali hesapla
            toplam=fabs(toplam);
            printf("\nSimpson 3/8 kuralina gore integral degeri:%lf",toplam);
            break;
        }
 }


 // Trapez yöntemi ile integral hesaplama fonksiyonu
 void Trapez()
 {

    //Kullanıcıdan ifade alma ve tokenlere ayırma
    double x;
    char in[MAX_INPUT_SIZE];
    printf("Matematiksel ifadeyi girin:");
    scanf("\n%[^\n]", in); 
        
        char* tokens = tokenize(in);      
        int i=0,tmp=0,j=1;
        while(tokens[i]!='\0'&&tmp!=1)
        {
            if(tokens[i]=='x')
            {
                tmp=1;
            }
            i++;
        }
        if(tmp==0)
        {
            printf("girdiginiz ifade x icermemektedir\n");
            exit(0);
        }

        //integral için aralık alma
         double left;
         printf("Araligin baslangicini giriniz:");
         scanf("%lf",&left);
         x=left;
         exprtree* expression1 = parse(tokens,x);
         double leftdeg = calculate(expression1);
    
         double right;
         printf("Araligin bitisini giriniz:");
         scanf("%lf",&right);
         x=right;
         exprtree* expression2 = parse(tokens,x); 
         double rightdeg = calculate(expression2);

         printf("n degerini giriniz:");
         double n,yeniDeg,toplam=0;;
         scanf("%lf",&n);
         double h=(right-left)/n;// Adım boyutunu hesapla
         x=left;
         for(i=1;i<n;i++) // İlk ve son terim dışındaki terimler için döngü
         {
            x=x+h;
            exprtree* expression3 = parse(tokens,x); 
            yeniDeg = calculate(expression3);
            toplam=toplam+yeniDeg;
            free_exprtree(expression3);
         }
         toplam=h*((rightdeg+leftdeg)/2+toplam);// Trapez kuralına göre integrali hesapla
         toplam=fabs(toplam);
         printf("Integral degeri:%lf\n",toplam);

 }


 void Gregory_Newton()
 {
    int n, i, j;
    printf("X ve Y sirali ikililerinin sayisini giriniz: ");
    scanf("%d", &n);

    double *x = (double*)malloc(n*sizeof(double));
    double *y = (double*)malloc(n*sizeof(double));

    printf("X ve Y degerlerini giriniz:\n");
    for (i=0;i<n;i++) {
        printf("X[%d]: ", i+1);
        scanf("%lf", &x[i]);
        printf("Y[%d]: ", i+1);
        scanf("%lf", &y[i]);
    }

    // İleri farklar tablosunu oluşturma
    double **IleriFark = (double**)malloc(n*sizeof(double *));
    for (i=0;i<n;i++) {
        IleriFark[i] = (double*)malloc(n*sizeof(double));
    }

    for (i=0;i<n;i++) {
        IleriFark[i][0] = y[i];
    }

    for (j=1;j<n;j++) {
        for (i=0;i<n-j;i++) {
            IleriFark[i][j] = IleriFark[i+1][j-1]-IleriFark[i][j-1];
        }
    }

    printf("\nIleri farklar tablosu:\n");
    for (i=0;i<n;i++) {
        printf("%d: ",i);
        for (j=0;j<n-i;j++) {
            printf("%lf ",IleriFark[i][j]);
        }
        printf("\n");
    }

    double value;// İnterpolasyon yapılacak değer
    printf("Hangi degerde interpolasyon yapmak istersiniz? ");
    scanf("%lf", &value);// İnterpolasyon değerini al

    double sonuc=IleriFark[0][0];
    double tmp=1.0;//her yeni gelecek (x-xi) degeri için kullanılır ve her döngüde tekrar tekrar çarpılır

    for (i=1;i<n;i++) 
    {
        tmp *=(value-x[i-1])/(i);   // (x - x0)(x - x1)...(x - xi-1) / (i!)
        sonuc +=tmp*IleriFark[0][i];  // Her yeni terim eklenir
    }
    printf("Interpolasyon sonucu: %lf\n", sonuc);

    for (i=0;i<n;i++) 
    {
        free(IleriFark[i]);
    }
    free(IleriFark);
    free(x);
    free(y);
 }