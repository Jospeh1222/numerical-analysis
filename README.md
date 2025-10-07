# 📌 Sayısal Yöntemler Uygulaması (C)

Bu proje, C dili kullanılarak geliştirilmiş bir **sayısal yöntemler hesaplama aracıdır**. Kullanıcıdan alınan fonksiyon ve parametrelere göre çeşitli kök bulma, türev, integral, matris ve enterpolasyon yöntemlerini uygulayabilir.

## 📚 İçindekiler
- [Özellikler](#-özellikler)
- [Desteklenen Fonksiyonlar](#-desteklenen-fonksiyonlar)
- [Kullanım](#-kullanım)
- [Yöntemler](#-yöntemler)
- [Örnek Girdiler](#-örnek-girdiler)
- [Kurulum](#-kurulum)
- [Geliştirici](#-geliştirici)

---

## 🚀 Özellikler
- Kök bulma yöntemleri (Bisection, Regula-Falsi, Newton-Raphson)  
- Matris işlemleri (Ters alma, Gauss Eliminasyon, Gauss-Seidel)  
- Sayısal türev ve integral hesaplamaları (Simpson, Trapez yöntemleri)  
- Gregory-Newton enterpolasyonu  
- Polinom, üstel, logaritmik, trigonometrik ve ters trigonometrik fonksiyon desteği  
- Hatalı girişlerde güvenli çalışma ve menü tabanlı etkileşimli arayüz

---

## 🧮 Desteklenen Fonksiyonlar

Program, aşağıdaki türde fonksiyonları anlayabilir:

| Tür | Örnek Girdi |
|-----|-------------|
| Polinomik | `3*x^3+5*x^2-6` |
| Logaritmik | `Log_e(x^3+6)` |
| Trigonometrik | `Sin(60)+cos(45*x)` |
| Ters Trigonometrik | `Arctan(1)+arcsin(0.5)` |

> ⚠️ Trigonometrik fonksiyonlarda parantez içi değerler **açı cinsindendir**.

---

## 📝 Kullanım

Program başlatıldığında bir **ana menü** açılır.  
Yapmak istediğiniz işlemi seçip, istenen parametreleri girerek hesaplamayı başlatabilirsiniz.  
`0` tuşlanana kadar veya geçersiz bir giriş yapılmadıkça program çalışmaya devam eder.

---

## 📌 Yöntemler

Aşağıdaki yöntemler desteklenmektedir:

1. Bisection Yöntemi  
2. Regula-Falsi Yöntemi  
3. Newton-Raphson Yöntemi  
4. NxN’lik Bir Matrisin Tersi  
5. Gauss Eliminasyon Yöntemi  
6. Gauss-Seidel Yöntemi  
7. Sayısal Türev  
8. Simpson Yöntemi  
9. Trapez Yöntemi  
10. Gregory-Newton Enterpolasyonu

Her yöntem için program, ilgili parametreleri kullanıcıdan alır (başlangıç-bitiş değerleri, hata miktarı, matris elemanları vb.).

---

## 🧪 Örnek Girdiler

### 🔸 Polinom
```
Girdi: 3*x^3+5*x^2-6
```

### 🔸 Logaritmik
```
Girdi: Log_e(x^3+6)
```

### 🔸 Trigonometrik
```
Girdi: Sin(60)+cos(45*x)
```

### 🔸 Matris (3x3)
```
[5 ,2 ,-4 :1 ,4 ,2: 2 ,3 ,6]
```

---

## 🛠️ Kurulum

1. Projeyi klonla:
```bash
git clone https://github.com/kullaniciadi/sayisal-yontemler-c.git
cd sayisal-yontemler-c
```

2. Derle:
```bash
gcc main.c -o sayisal
```

3. Çalıştır:
```bash
./sayisal
```

> 💡 Kod Visual Studio Code üzerinde geliştirilmiştir.

---

## 👨‍💻 Geliştirici

**Yusuf Başar Gündüz**  
📧 basar.gunduz@std.yildiz.edu.tr  
🎓 Yıldız Teknik Üniversitesi - Bilgisayar Mühendisliği

---



