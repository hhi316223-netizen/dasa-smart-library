#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <cstdlib>
using namespace std;

struct NguoiCho {
    string maNguoi;
    int mucUuTien;
    int ngayDangKy, gioDangKy;
    int soThuTu;
    NguoiCho* next;
};

struct PhieuMuon {
    string maTaiLieu, maNguoiMuon;
    int hanTra, gioTra;
    NguoiCho* hangCho;
    PhieuMuon* prev;
    PhieuMuon* next;
};

struct ThuVien {
    PhieuMuon* dau;
    PhieuMuon* cuoi;
    unordered_map<string, PhieuMuon*> theoMa;
    int soThuTuTiepTheo;
};

void khoiTao(ThuVien& tv) {
    tv.dau = tv.cuoi = NULL;
    tv.theoMa.clear();
    tv.soThuTuTiepTheo = 1;
}

bool hopLe(int ngay, int gio) {
    int y = ngay / 10000, m = ngay / 100 % 100, d = ngay % 100;
    int h = gio / 10000, ph = gio / 100 % 100, s = gio % 100;
    int soNgay[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) soNgay[2] = 29;
    return y >= 1900 && y <= 2100 && m >= 1 && m <= 12 && d >= 1 && d <= soNgay[m]
        && gio >= 0 && h <= 23 && ph <= 59 && s <= 59;
}

void inThoiGian(int ngay, int gio) {
    cout << setfill('0') << setw(2) << ngay % 100 << "/" << setw(2) << ngay / 100 % 100 << "/" << ngay / 10000
         << " " << setw(2) << gio / 10000 << ":" << setw(2) << gio / 100 % 100 << ":" << setw(2) << gio % 100
         << setfill(' ');
}

bool dungTruoc(PhieuMuon* a, PhieuMuon* b) {
    if (a->hanTra != b->hanTra) return a->hanTra < b->hanTra;
    if (a->gioTra != b->gioTra) return a->gioTra < b->gioTra;
    return a->maTaiLieu < b->maTaiLieu;
}

bool uuTienHon(NguoiCho* a, NguoiCho* b) {
    if (a->mucUuTien != b->mucUuTien) return a->mucUuTien < b->mucUuTien;
    if (a->ngayDangKy != b->ngayDangKy) return a->ngayDangKy < b->ngayDangKy;
    if (a->gioDangKy != b->gioDangKy) return a->gioDangKy < b->gioDangKy;
    return a->soThuTu < b->soThuTu;
}

PhieuMuon* timPhieu(ThuVien& tv, string ma) {
    if (tv.theoMa.count(ma) == 0) return NULL;
    return tv.theoMa[ma];
}

void chenPhieu(ThuVien& tv, PhieuMuon* p) {
    PhieuMuon* q = tv.cuoi;
    while (q != NULL && dungTruoc(p, q)) q = q->prev;
    p->prev = q;
    p->next = (q != NULL) ? q->next : tv.dau;
    if (p->next != NULL) p->next->prev = p; else tv.cuoi = p;
    if (q != NULL) q->next = p; else tv.dau = p;
}

void goPhieu(ThuVien& tv, PhieuMuon* p) {
    if (p->prev != NULL) p->prev->next = p->next; else tv.dau = p->next;
    if (p->next != NULL) p->next->prev = p->prev; else tv.cuoi = p->prev;
}

void chenNguoi(PhieuMuon* s, NguoiCho* n) {
    if (s->hangCho == NULL || uuTienHon(n, s->hangCho)) { n->next = s->hangCho; s->hangCho = n; return; }
    NguoiCho* q = s->hangCho;
    while (q->next != NULL && uuTienHon(q->next, n)) q = q->next;
    n->next = q->next;
    q->next = n;
}

bool muonSach(ThuVien& tv, string ma, string nguoi, int han, int gio) {
    if (ma == "" || nguoi == "" || !hopLe(han, gio) || timPhieu(tv, ma) != NULL) return false;
    PhieuMuon* p = new PhieuMuon{ma, nguoi, han, gio, NULL, NULL, NULL};
    chenPhieu(tv, p);
    tv.theoMa[ma] = p;
    return true;
}

bool giaHan(ThuVien& tv, string ma, int hanMoi, int gioMoi) {
    PhieuMuon* p = timPhieu(tv, ma);
    if (p == NULL || p->hangCho != NULL || !hopLe(hanMoi, gioMoi)) return false;
    goPhieu(tv, p);
    p->hanTra = hanMoi;
    p->gioTra = gioMoi;
    chenPhieu(tv, p);
    return true;
}

int demTreHan(ThuVien& tv, int homNay) {
    int dem = 0;
    for (PhieuMuon* p = tv.dau; p != NULL && p->hanTra < homNay; p = p->next) dem++;
    return dem;
}

vector<PhieuMuon*> layTreNhat(ThuVien& tv, int homNay, int k) {
    vector<PhieuMuon*> kq;
    for (PhieuMuon* p = tv.dau; p != NULL && (int)kq.size() < k && p->hanTra < homNay; p = p->next)
        kq.push_back(p);
    return kq;
}

bool dangKyCho(ThuVien& tv, string nguoi, string ma, int muc, int ngay, int gio) {
    PhieuMuon* s = timPhieu(tv, ma);
    if (s == NULL || nguoi == "" || muc < 1 || muc > 5 || !hopLe(ngay, gio)) return false;
    if (s->maNguoiMuon == nguoi) return false;
    for (NguoiCho* q = s->hangCho; q != NULL; q = q->next)
        if (q->maNguoi == nguoi) return false;
    chenNguoi(s, new NguoiCho{nguoi, muc, ngay, gio, tv.soThuTuTiepTheo++, NULL});
    return true;
}

bool huyDangKy(ThuVien& tv, string nguoi, string ma) {
    PhieuMuon* s = timPhieu(tv, ma);
    if (s == NULL) return false;
    NguoiCho* truoc = NULL;
    NguoiCho* q = s->hangCho;
    while (q != NULL && q->maNguoi != nguoi) { truoc = q; q = q->next; }
    if (q == NULL) return false;
    if (truoc == NULL) s->hangCho = q->next; else truoc->next = q->next;
    delete q;
    return true;
}

int viTriTrongHang(ThuVien& tv, string nguoi, string ma) {
    PhieuMuon* s = timPhieu(tv, ma);
    int i = 1;
    for (NguoiCho* q = (s != NULL) ? s->hangCho : NULL; q != NULL; q = q->next, i++)
        if (q->maNguoi == nguoi) return i;
    return 0;
}

bool traSach(ThuVien& tv, string ma, int hanMoi, int gioMoi, string& nguoiNhan) {
    nguoiNhan = "";
    PhieuMuon* p = timPhieu(tv, ma);
    if (p == NULL) return false;
    goPhieu(tv, p);
    if (p->hangCho == NULL) {
        tv.theoMa.erase(ma);
        delete p;
        return true;
    }
    NguoiCho* dauHang = p->hangCho;
    nguoiNhan = dauHang->maNguoi;
    p->maNguoiMuon = dauHang->maNguoi;
    p->hanTra = hanMoi;
    p->gioTra = gioMoi;
    p->hangCho = dauHang->next;
    delete dauHang;
    chenPhieu(tv, p);
    return true;
}


int napTuFile(ThuVien& tv, string tenFile) {
    ifstream f(tenFile);
    if (!f) return -1;
    string dong;
    int dem = 0;
    while (getline(f, dong)) {
        stringstream ss(dong);
        string ma, nguoi, han, gio;
        getline(ss, ma, ',');
        getline(ss, nguoi, ',');
        getline(ss, han, ',');
        getline(ss, gio, ',');
        if (muonSach(tv, ma, nguoi, atoi(han.c_str()), atoi(gio.c_str()))) dem++;
    }
    return dem;
}

void giaiPhong(ThuVien& tv) {
    while (tv.dau != NULL) {
        PhieuMuon* p = tv.dau;
        tv.dau = p->next;
        while (p->hangCho != NULL) {
            NguoiCho* q = p->hangCho;
            p->hangCho = q->next;
            delete q;
        }
        delete p;
    }
    khoiTao(tv);
}

void inDanhSach(ThuVien& tv, int homNay) {
    int i = 0, soTre = demTreHan(tv, homNay);
    for (PhieuMuon* p = tv.dau; p != NULL; p = p->next, i++) {
        if (i == soTre) cout << "    ---- bien tre han: " << soTre << " phieu tre ----\n";
        cout << "    [" << i << "] han ";
        inThoiGian(p->hanTra, p->gioTra);
        cout << "  " << p->maTaiLieu << "  " << p->maNguoiMuon << "\n";
    }
    if (i == soTre) cout << "    ---- bien tre han: " << soTre << " phieu tre ----\n";
}

void inHangCho(ThuVien& tv, string ma) {
    PhieuMuon* s = timPhieu(tv, ma);
    cout << "    Hang cho " << ma << ":" << (s == NULL || s->hangCho == NULL ? " khong ai cho" : "") << "\n";
    int i = 1;
    for (NguoiCho* q = (s != NULL) ? s->hangCho : NULL; q != NULL; q = q->next, i++) {
        cout << "      " << i << ". " << q->maNguoi << "  muc " << q->mucUuTien << "  dang ky ";
        inThoiGian(q->ngayDangKy, q->gioDangKy);
        cout << "\n";
    }
}

int main() {
    int homNay = 20261015;
    ThuVien tv;
    khoiTao(tv);

    cout << "===== MC2: DANH SACH TRE HAN =====\n";
    cout << "\n1) Nap phieu muon tu file phieumuon.csv:\n";
    int soPhieu = napTuFile(tv, "phieumuon.csv");
    if (soPhieu == -1) {
        cout << "    Khong mo duoc file phieumuon.csv\n";
        return 1;
    }
    cout << "    Da nap " << soPhieu << " phieu\n";
    inDanhSach(tv, homNay);

    cout << "\n2) SV006 muon BK-CSDL-05, han 29/10 17:00 -> noi cuoi:\n";
    muonSach(tv, "BK-CSDL-05", "SV006", 20261029, 170000);
    inDanhSach(tv, homNay);

    cout << "\n3) So phieu tre: " << demTreHan(tv, homNay) << ". 2 phieu tre nang nhat:\n";
    vector<PhieuMuon*> top = layTreNhat(tv, homNay, 2);
    for (int i = 0; i < (int)top.size(); i++) {
        cout << "    " << i + 1 << ". " << top[i]->maTaiLieu << " - " << top[i]->maNguoiMuon << " - han ";
        inThoiGian(top[i]->hanTra, top[i]->gioTra);
        cout << "\n";
    }

    string nguoiNhan;
    cout << "\n4) SV002 tra BK-GT-07 (khong ai cho) -> go phieu ra:\n";
    traSach(tv, "BK-GT-07", 0, 0, nguoiNhan);
    inDanhSach(tv, homNay);

    cout << "\n5) GV010 gia han BK-OOP-02 len 19/10 08:00:\n";
    giaHan(tv, "BK-OOP-02", 20261019, 80000);
    inDanhSach(tv, homNay);

    cout << "\n===== FR1: HANG CHO UU TIEN (muc 1 = cao nhat) =====\n";
    cout << "\n6) 4 nguoi dang ky cho BK-CSDL-05 (dang duoc SV006 muon):\n";
    dangKyCho(tv, "SV010", "BK-CSDL-05", 5, 20261015, 90000);
    dangKyCho(tv, "SV011", "BK-CSDL-05", 3, 20261015, 90500);
    dangKyCho(tv, "SV012", "BK-CSDL-05", 3, 20261015, 90200);
    dangKyCho(tv, "GV020", "BK-CSDL-05", 1, 20261015, 100000);
    inHangCho(tv, "BK-CSDL-05");

    cout << "\n7) Cac truong hop bi tu choi:\n";
    cout << "    Cho BK-GT-07 (dang tren ke):    " << (dangKyCho(tv, "SV013", "BK-GT-07", 2, 20261015, 90000) ? "DUOC" : "TU CHOI") << "\n";
    cout << "    Muc uu tien 6:                  " << (dangKyCho(tv, "SV014", "BK-CSDL-05", 6, 20261015, 90000) ? "DUOC" : "TU CHOI") << "\n";
    cout << "    Ngay 30/02/2026:                " << (dangKyCho(tv, "SV015", "BK-CSDL-05", 3, 20260230, 90000) ? "DUOC" : "TU CHOI") << "\n";
    cout << "    SV010 dang ky lan 2:            " << (dangKyCho(tv, "SV010", "BK-CSDL-05", 1, 20261015, 110000) ? "DUOC" : "TU CHOI") << "\n";
    cout << "    SV006 cho sach minh dang giu:   " << (dangKyCho(tv, "SV006", "BK-CSDL-05", 1, 20261015, 110000) ? "DUOC" : "TU CHOI") << "\n";
    cout << "    SV006 gia han khi co nguoi cho: " << (giaHan(tv, "BK-CSDL-05", 20261105, 0) ? "DUOC" : "TU CHOI") << "\n";

    cout << "\n8) SV011 dang dung thu " << viTriTrongHang(tv, "SV011", "BK-CSDL-05") << " trong hang cho\n";

    cout << "\n9) SV006 tra BK-CSDL-05 -> sach chuyen cho nguoi uu tien nhat, han moi 29/10:\n";
    traSach(tv, "BK-CSDL-05", 20261029, 0, nguoiNhan);
    cout << "    Sach chuyen cho: " << nguoiNhan << "\n";
    inDanhSach(tv, homNay);
    inHangCho(tv, "BK-CSDL-05");

    cout << "\n10) SV011 huy dang ky:\n";
    huyDangKy(tv, "SV011", "BK-CSDL-05");
    inHangCho(tv, "BK-CSDL-05");

    giaiPhong(tv);
    return 0;
}
