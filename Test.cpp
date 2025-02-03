// Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <utility>
#include <string>
#include <string_view> // !! C++17
#include <vector>
#include <array>

#include <fstream>
#include <chrono>
#include <functional>
#include <execution> // !! C++17

#include <iomanip>
// std::for_each(
#include <algorithm>

#include <iterator>
#include <optional> // !! C++17
#include <atomic>

struct my_line_iterator {
    using iterator_category = std::input_iterator_tag;
    using value_type        = std::string_view;
    using difference_type   = std::ptrdiff_t;
    using reference_type    = const value_type&;
    using pointer_type      = const value_type*;

    // Definition des Endes
    my_line_iterator() { };
    my_line_iterator(std::string_view const& input) : theText(input) { ++*this; };

    my_line_iterator& operator = (std::string_view const& vw) {
        theText = vw;
        start_pos = 0u;
        end_pos = 0u;
        return *this;
    }

    my_line_iterator& operator = (my_line_iterator const& ref) {
        theText = ref.theText;
        theLine = ref.theLine;
        start_pos = ref.start_pos;
        end_pos = ref.end_pos;
        return *this;
    }

    reference_type operator*() const { return theLine; }
    pointer_type operator->() const { return &theLine; }

    // Prefix Operator
    my_line_iterator& operator++() {
        if (theText) {
            end_pos = theText->find('\n', start_pos);
            if (end_pos != std::string_view::npos) {
                theLine = theText->substr(start_pos, end_pos - start_pos);
                start_pos = end_pos + 1;
            }
            else {
                theText = { };
                // theText = std::nullopt;
            }
        }
        return *this;
    }

    // durch (int) wird der Postfix Operator
    my_line_iterator operator++(int) {
        auto elem(*this);
        ++*this;
        return elem;
    }

    friend bool operator == (my_line_iterator const& lhs, my_line_iterator const& rhs) {
        return lhs.theText == rhs.theText;
    }

    friend bool operator != (my_line_iterator const& lhs, my_line_iterator const& rhs) {
        return !(lhs == rhs);
    }

private:
    std::optional<std::string_view> theText = { };
    std::string_view theLine;
    size_t start_pos = 0u, end_pos = 0u;
};

template <typename tty, typename fty, typename... Args>
inline auto Call(tty& time, fty function, Args... args) {
    auto func_start = std::chrono::high_resolution_clock::now();
    auto ret = function(std::forward<Args>(args)...);
    auto func_ende = std::chrono::high_resolution_clock::now();
    time = std::chrono::duration_cast<tty>(func_ende - func_start);
    return ret;
}

// Using template oder alias template
template <typename ty>
using Location = std::pair<ty, ty>;

template <typename ty>
std::ostream& operator << (std::ostream& out, Location<ty> const& data) {
    out << "(" << data.first << ", " << data.second << ")";
    return out;
}



/** Klasse mit den notwendigen Daten für eine Adresse (ohne geografische Position)
*/
class TAddress {
    friend void swap(TAddress& lhs, TAddress& rhs) {
        swap(lhs.strCity, rhs.strCity);
        swap(lhs.strStreet, rhs.strStreet);
        swap(lhs.strStreetNumber, rhs.strStreetNumber);
        swap(lhs.strZipCode, rhs.strZipCode);
        swap(lhs.strUrbanUnit, rhs.strUrbanUnit);
        swap(lhs.strUrbanUnit_Old, rhs.strUrbanUnit_Old);
        swap(lhs.strDistrict, rhs.strDistrict);
    }
private: 
    std::string strCity          = ""; ///< Stadt, an der sich die Adresse befindet
    std::string strStreet        = "";
    std::string strStreetNumber  = "";
    std::string strZipCode       = "";
    std::string strUrbanUnit     = "";
    std::string strUrbanUnit_Old = "";
    std::string strDistrict      = "";
public:
    TAddress(void) {
        _init();
    }
    /* Kopier-Konstruktor */
    TAddress(TAddress const& ref) {
        _copy(ref);
    }

    TAddress(TAddress&& ref) noexcept {
        _swap(std::forward<TAddress>(ref));
    }

    virtual ~TAddress(void) = default;

    virtual TAddress* create() { return new TAddress; }

    TAddress& operator = (TAddress const& ref) {
        copy(ref);
        return *this;
    }

    // Regel der 5 rValue Zuweisungsoperator
    TAddress& operator = (TAddress&& ref) noexcept {
        swap(*this, ref);
        return *this;
    }

    virtual void init() { _init(); }
    virtual void copy(TAddress const& ref) { _copy(ref); }

    /// @name Selektoren
    /// {
    std::string const& City(void) const { return strCity; }
    std::string const& Street(void) const { return strStreet; }
    std::string const& StreetNumber(void) const { return strStreetNumber; }
    std::string const& ZipCode(void) const { return strZipCode; }
    std::string const& UrbanUnit(void) const { return strUrbanUnit; }
    std::string const& UrbanUnit_Old(void) const { return strUrbanUnit_Old; }
    std::string const& District(void) const { return strDistrict; }
    /// }

    /// @name Manipulatoren
    /// {
    void City(std::string const& newVal) { strCity = newVal; }
    void Street(std::string const& newVal) { strStreet = newVal; }
    void StreetNumber(std::string const& newVal) { strStreetNumber = newVal; }
    void ZipCode(std::string const& newVal) { strZipCode = newVal; }
    void UrbanUnit(std::string const& newVal) { strUrbanUnit = newVal; }
    void UrbanUnit_Old(std::string const& newVal) { strUrbanUnit_Old = newVal; }
    void District(std::string const& newVal) { strDistrict = newVal; }

    /// rvalues
    /// {
    void City(std::string&& newVal) { strCity = std::forward<std::string>(newVal); }
    void Street(std::string&& newVal) { strStreet = std::forward<std::string>(newVal); }
    void StreetNumber(std::string&& newVal) { strStreetNumber = std::forward<std::string>(newVal); }
    void ZipCode(std::string&& newVal) { strZipCode = std::forward<std::string>(newVal); }
    void UrbanUnit(std::string&& newVal) { strUrbanUnit = std::forward<std::string>(newVal); }
    void UrbanUnit_Old(std::string&& newVal) { strUrbanUnit_Old = std::forward<std::string>(newVal); }
    void District(std::string&& newVal) { strDistrict = std::forward<std::string>(newVal); }

    /// String_Views<summary>
    /// 
    void City(std::string_view const& newVal) { strCity = std::move(std::string{ newVal.data(), newVal.size() }); }
    void Street(std::string_view const& newVal) { strStreet = std::move(std::string{ newVal.data(), newVal.size() }); }
    void StreetNumber(std::string_view const& newVal) { strStreetNumber = std::move(std::string{ newVal.data(), newVal.size() }); }
    void ZipCode(std::string_view const& newVal) { strZipCode = std::move(std::string{ newVal.data(), newVal.size() }); }
    void UrbanUnit(std::string_view const& newVal) { strUrbanUnit = std::move(std::string{ newVal.data(), newVal.size() }); }
    void UrbanUnit_Old(std::string_view const& newVal) { strUrbanUnit_Old = std::move(std::string{ newVal.data(), newVal.size() }); }
    void District(std::string_view const& newVal) { strDistrict = std::move(std::string{ newVal.data(), newVal.size() }); }

private:
    void _init(void) {
        strCity          = "";
        strStreet        = "";
        strStreetNumber  = "";
        strZipCode       = "";
        strUrbanUnit     = "";
        strUrbanUnit_Old = "";
        strDistrict      = "";
    }
    void _copy(TAddress const& ref) {
        City(ref.City());
        Street(ref.Street());
        StreetNumber(ref.StreetNumber());
        ZipCode(ref.ZipCode());
        UrbanUnit(ref.UrbanUnit());
        UrbanUnit_Old(ref.UrbanUnit_Old());
        District(ref.District());
    }
    void _swap(TAddress&& ref) noexcept {
        swap(strCity, ref.strCity);
        swap(strStreet, ref.strStreet);
        swap(strStreetNumber, ref.strStreetNumber);
        swap(strZipCode, ref.strZipCode);
        swap(strUrbanUnit, ref.strUrbanUnit);
        swap(strUrbanUnit_Old, ref.strUrbanUnit_Old);
        swap(strDistrict, ref.strDistrict);
    }
};


template <typename ty>
class TData : public TAddress {
    friend void swap(TData& lhs, TData& rhs) {
        swap(static_cast<TAddress&>(lhs), static_cast<TAddress&>(rhs));
        swap(lhs.mLoc, rhs.mLoc);
    }
private:
    Location<ty> mLoc;
public:
    TData(void) : TAddress() { _init(); }
    TData(TData const& ref) : TAddress(ref) { _copy(ref); }

    TData(TData&& ref) noexcept : TAddress(ref) { _swap(std::forward<TData>(ref)); }

    virtual ~TData(void) override { }

    // jetzt wurde Regel der 3 dazu gebaut; jetzt noch um rValue zur Regel der 5 erweitern
    TData& operator = (TData const& ref) {
        copy(ref);
        return *this;
    }

    // rValue Zuweisungsoperator (Piratenoperator)
    TData& operator = (TData&& ref) noexcept {
        _swap(std::forward<TData>(ref));
        return *this;
    }

    virtual TAddress* create() { return new TData<ty>; }
    virtual void init(void) { TAddress::init(); _init(); }
    virtual void copy(TAddress const& ref) { 
        TAddress::copy(ref);
        try {
            _copy(dynamic_cast<TData const&>(ref));
        }
        catch (std::bad_cast& ex) { ; }
    }

    /* fuer lvalue */
    operator Location<ty> & ()  { return mLoc; }
    /* nur lesbar: konstante Referenz */
    operator Location<ty> const& () const { return mLoc; }

    ty const& Latitude(void) const { return mLoc.first; }
    ty const& Longitude(void) const { return mLoc.second; }

    void Latitude(ty const& newVal) { mLoc.first = newVal; }
    void Longitude(ty const& newVal) { mLoc.second = newVal; }

    void Latitude(std::string const& newVal) { auto result = std::from_chars(newVal.data(), newVal.data() + newVal.size(), mLoc.first); } //mLoc.first = std::stod(newVal); }
    void Longitude(std::string const& newVal) { auto result = std::from_chars(newVal.data(), newVal.data() + newVal.size(), mLoc.second); } //mLoc.second = std::stod(newVal); }

    void Latitude(std::string&& newVal) { auto result = std::from_chars(newVal.data(), newVal.data() + newVal.size(), mLoc.first); } //mLoc.first = std::stod(newVal); }
    void Longitude(std::string&& newVal) { auto result = std::from_chars(newVal.data(), newVal.data() + newVal.size(), mLoc.second); } //mLoc.second = std::stod(newVal); }

    // String_Views
    void Latitude(std::string_view const& newVal) { auto result = std::from_chars(newVal.data(), newVal.data() + newVal.size(), mLoc.first); } //mLoc.first = std::stod(newVal); }
    void Longitude(std::string_view const& newVal) { auto result = std::from_chars(newVal.data(), newVal.data() + newVal.size(), mLoc.second); } //mLoc.second = std::stod(newVal); }

private:
    // early binding Funktionen, behandeln nur die lokalen Datenelemente
    void _init(void) { mLoc = { 0.0, 0.0 }; }
    void _copy(TData const& ref) { mLoc = ref.mLoc; }
    void _swap(TData&& ref) noexcept { swap(mLoc, ref.mLoc); }
};

template <typename ty>
using data_vector = std::vector<TData<ty>>;


// named return Wert Optimierung
std::vector<std::string> tokenize(std::string const& strInput, std::string const& strDelimiter, int iSize = -1) {
    std::vector<std::string> vecData;
    if (iSize >= 0) vecData.reserve(iSize);
    size_t iPos = 0, iEnd;
    do {
        iEnd = strInput.find(strDelimiter, iPos);
        std::string strPart = strInput.substr(iPos, iEnd - iPos);
        vecData.emplace_back(std::forward<std::string>(strPart));
        iPos = iEnd + strDelimiter.length();
    } while (iEnd != std::string::npos);
    return vecData;
}


std::string ReadHlp(std::string const& strRow, size_t& iPos) {
    size_t iEnd = strRow.find(";", iPos), iOldPos(iPos);
    iPos = iEnd + 1;
    return strRow.substr(iPos, iEnd - iOldPos);
   }

using func_vector = std::vector<std::function<void(TData<double>&, std::string const&)>>;

const func_vector funcs = {
    /* [] = lamda */
    [](TData<double>& data, std::string const& val) { data.City(val); },
    //nullptr;
    [](TData<double>& data, std::string const& val) { data.Street(val); },
    [](TData<double>& data, std::string const& val) { data.StreetNumber(val); },
    [](TData<double>& data, std::string const& val) { data.ZipCode(val); },
    [](TData<double>& data, std::string const& val) { data.UrbanUnit(val); },
    [](TData<double>& data, std::string const& val) { data.UrbanUnit_Old(val); },
    [](TData<double>& data, std::string const& val) { data.District(val); },
    [](TData<double>& data, std::string const& val) { data.Latitude(val); },
    [](TData<double>& data, std::string const& val) { data.Longitude(val); },
    [](TData<double>&, std::string const&) { throw std::runtime_error("unexpected number of elements."); }
};

using func_vector_vw = std::vector<std::function<void(TData<double>&, std::string_view const&)>>;

const func_vector_vw funcs_vw = {
    /* [] = lamda */
    [](TData<double>& data, std::string_view const& val) { data.City(val); },
    //nullptr;
    [](TData<double>& data, std::string_view const& val) { data.Street(val); },
    [](TData<double>& data, std::string_view const& val) { data.StreetNumber(val); },
    [](TData<double>& data, std::string_view const& val) { data.ZipCode(val); },
    [](TData<double>& data, std::string_view const& val) { data.UrbanUnit(val); },
    [](TData<double>& data, std::string_view const& val) { data.UrbanUnit_Old(val); },
    [](TData<double>& data, std::string_view const& val) { data.District(val); },
    [](TData<double>& data, std::string_view const& val) { data.Latitude(val); },
    [](TData<double>& data, std::string_view const& val) { data.Longitude(val); },
    [](TData<double>&, std::string_view const&) { throw std::runtime_error("unexpected number of elements."); }
};

template <typename ty>
size_t Read_0(data_vector<ty>& vData, func_vector const& funcs, std::istream& ifs) {
    // clear
    // hiermit wird das dynamische Vergrößern des vectors verhindert
//    vData.reserve(std::count(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>(), '\n'));
    // nach 1. Einlesen zurück auf Anfang
//    ifs.seekg(0, std::ios::beg);
    size_t iLineCnt = 0u;
    std::string strRow;
    while (std::getline(ifs, strRow)) {
        TData<double> data;
        auto input = tokenize(strRow, ";", 9);
        //size_t iCount = 0u;
        // C++ 17 range-based for (das element : input)
        for (size_t iCnt = 0u; auto const& element : input) { funcs[iCnt](data, input[iCnt]); ++iCnt; }
        vData.emplace_back(std::forward<TData<double>>(data));
        ++iLineCnt;
    }
    return iLineCnt;
}

// Erklärung Evolution von C++ 6.3.2023 46:09 min bis 1:06:27 
template<typename ty>
size_t Read_1(data_vector<ty>& vData, func_vector const& funcs, std::string const& buffer) {
    size_t pos = 0u;
    // C++17 structured binding auto [pos, end] = std::make_pair
    // zeilenweise suchen
    // std::string liefert seine Endposition in "std::string::npos"
    // C++ liefert , Operator: wird hier in der Neu-Initialisierung angewendet: pos = end + 1 UND(,) end = ...
    for (auto [pos, end] = std::make_pair( 0u, buffer.find('\n') ); end != std::string::npos; pos = end + 1, end = buffer.find('\n', pos)) {
        size_t iCnt = 0u;
        TData<ty> data;
        // 1 ganze Zeile mit dem func vector tokenizen
        do {
            auto tmp = buffer.find(';', pos);
            // Spezialfall letztes Element einer Zeile
            if (tmp > end) tmp = end;
            funcs[iCnt++](data, buffer.substr(pos, tmp - pos));
            pos = tmp + 1;
        } while (pos < end);
        // von data nach vData 
        vData.emplace_back(std::forward<TData<ty>>(data));
    }
//    return pos;
}

template<typename ty>
size_t Read_2(data_vector<ty>& vData, func_vector_vw const& funcs, std::string const& buffer) {
    //size_t pos = 0u;
    std::string_view view(buffer.c_str(), buffer.size());
    // C++17 structured binding auto [pos, end] = std::make_pair
    // zeilenweise suchen
    // std::string liefert seine Endposition in "std::string::npos"
    // C++ liefert , Operator: wird hier in der Neu-Initialisierung angewendet: pos = end + 1 UND(,) end = ...
    for (auto [pos, end] = std::make_pair(0u, view.find('\n')); end != std::string_view::npos; pos = end + 1, end = view.find('\n', pos)) {
        size_t iCnt = 0u;
        TData<ty> data;
        // 1 ganze Zeile mit dem func vector tokenizen
        do {
            auto tmp = view.find(';', pos);
            // Spezialfall letztes Element einer Zeile
            if (tmp > end) tmp = end;
            funcs[iCnt++](data, view.substr(pos, tmp - pos));
            pos = tmp + 1;
        } while (pos < end);
        // von data nach vData 
        vData.emplace_back(std::forward<TData<ty>>(data));
    }
    //    return pos;
}

template <typename ty>
inline void Write(data_vector<ty>& vData, std::ostream& os) {
    os.setf(std::ios::showpoint);
    os.setf(std::ios::fixed);
    os.precision(6);
    std::for_each(vData.cbegin(), vData.cend(), [&os](auto const& val) {
        os << val.ZipCode() << " " << val.City() << " / " << val.UrbanUnit() << ", "
            << val.Street() << " " << val.StreetNumber() << " -> (" << val.Latitude() << ", "
            << val.Longitude() << ")\n";
        });
}

// 100 pages als Lesepuffer definiert Windows pagesize 4 K
std::array<char, 4'096 * 100> buffer;

void Test1(std::string const& strFilename) {
    std::cout << "1st Test, Reading sequential with getline, vector dynamic increased. \n";
    data_vector<double> vecData;
    std::ifstream ifs(strFilename);
    ifs.rdbuf()->pubsetbuf(buffer.data(), buffer.size());
    if (!ifs.is_open()) {
        std::cerr << "File \"berlin_infos.dat\" can't open!\n";
        return;
    }

    std::chrono::milliseconds runtime;
    std::cout << Call(runtime, Read_0<double>, std::ref(vecData), std::cref(funcs), std::ref(ifs)) << " datasets read\n";
    std::cout << std::setw(12) << std::setprecision(3) << runtime.count() / 1000. << " sec\n";

    std::ofstream ofs("Testausgabe.txt");
    Write<double>(vecData, ofs);
}

// hiermit gibt es keine lästige dynamische Vergrößerung des vectors mehr
void Test2(std::string const& strFilename) {
    std::cout << "2nd Test, Reading about stringstream with getline, vector reserved.\n";
    std::ifstream ins(strFilename, std::ifstream::binary);
    if (!ins) {
        std::cerr << "file with name \"" << strFilename << "\" can't open.\n";
        return;
    }
    else {
        ins.seekg(0, std::ios::end);
        auto size = ins.tellg();
        ins.seekg(0, std::ios::beg);
        std::string strBuffer(size, '\0');
        // Puffer mit Daten füllen
        ins.read(strBuffer.data(), size);
        ins.close();
        data_vector<double> vData;
        //vData.reserve(std::count(strBuffer.begin(), strBuffer.end(), '\n'));
        vData.reserve(std::count(std::execution::par, strBuffer.begin(), strBuffer.end(), '\n'));
        std::istringstream iss(strBuffer);

        std::chrono::milliseconds runtime;
        std::cout << Call(runtime, Read_0<double>, std::ref(vData), std::cref(funcs), std::ref(iss)) << " datasets read\n";
        std::cout << std::setw(12) << std::setprecision(3) << runtime.count() / 1000. << " sec\n";

        std::ofstream ofs("Testausgabe.txt");
        Write<double>(vData, ofs);
    }
}

void Test3(std::string const& strFilename) {
    std::cout << "3rd Test, string operations, vector reserved.\n";
    std::ifstream ins(strFilename, std::ifstream::binary);
    if (!ins) {
        std::cerr << "file with name \"" << strFilename << "\" can't open.\n";
        return;
    }
    else {
        ins.seekg(0, std::ios::end);
        auto size = ins.tellg();
        // Lesezeiger wieder auf Anfang stellen
        ins.seekg(0, std::ios::beg);
        std::string strBuffer(size, '\0');
        // Puffer mit Daten füllen
        ins.read(strBuffer.data(), size);
        ins.close();
        data_vector<double> vData;
        //vData.reserve(std::count(strBuffer.begin(), strBuffer.end(), '\n'));
        vData.reserve(std::count(std::execution::par, strBuffer.begin(), strBuffer.end(), '\n'));

        std::chrono::milliseconds runtime;
        std::cout << Call(runtime, Read_1<double>, std::ref(vData), std::cref(funcs), std::cref(strBuffer)) << " datasets read\n";
        std::cout << std::setw(12) << std::setprecision(3) << runtime.count() / 1000. << " sec\n";

        std::ofstream ofs("Testausgabe.txt");
        Write<double>(vData, ofs);
    }
}

void Test4(std::string const& strFilename) {
    std::cout << "4th Test, string_view operations, vector reserved.\n";
    std::ifstream ins(strFilename, std::ifstream::binary);
    if (!ins) {
        std::cerr << "file with name \"" << strFilename << "\" can't open.\n";
        return;
    }
    else {
        ins.seekg(0, std::ios::end);
        auto size = ins.tellg();
        // Lesezeiger wieder auf Anfang stellen
        ins.seekg(0, std::ios::beg);
        std::string strBuffer(size, '\0');
        // Puffer mit Daten füllen
        ins.read(strBuffer.data(), size);
        ins.close();
        data_vector<double> vData;
        //vData.reserve(std::count(strBuffer.begin(), strBuffer.end(), '\n'));
        vData.reserve(std::count(std::execution::par, strBuffer.begin(), strBuffer.end(), '\n'));

        std::chrono::milliseconds runtime;
        std::cout << Call(runtime, Read_2<double>, std::ref(vData), std::cref(funcs_vw), std::cref(strBuffer)) << " datasets read\n";
        std::cout << std::setw(12) << std::setprecision(3) << runtime.count() / 1000. << " sec\n";

        std::ofstream ofs("Testausgabe.txt");
        Write<double>(vData, ofs);
    }
}

template <typename ty>
void Calculate(Location<ty> const& a, Location<ty> const& b) {
    std::cout << a << " -> " << b << "\n";
}

int main() {
    // Synchronisierung zwischen C stdin und C++ abschalten
    std::ios_base::sync_with_stdio(false);
    std::cout.setf(std::ios::showpoint);
    std::cout.setf(std::ios::fixed);
/*    Location<double> point = {2.0, 1.0};


    std::cout << "Hello World!\n";
    Calculate<double>(data, point);
 */
    Test4("berlin_infos.dat");
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
