#include "pch.h"
#include "RegEdit.h"

RegEdit::RegEdit(): m_values() {
}

RegEdit::~RegEdit() {
}

void RegEdit::Load(){
  try {
    this->m_values = getAllRegistryValues(this->m_subKey);
    for (const auto& [name, value] : m_values) {
      std::wcout << name << L" = ";
      if (std::holds_alternative<std::wstring>(value)) {
        std::wcout << std::get<std::wstring>(value);
      }
      else {
        std::wcout << std::get<DWORD>(value);
      }
      std::wcout << std::endl;
    }
  }
  catch (const std::exception& e) {
    std::wcerr << L"Failed to read registry values: " << e.what() << std::endl;
  }
}

RegistryStruct& RegEdit::GetValues() const {
  return m_values;
}

RegistryStruct RegEdit::getAllRegistryValues(const std::wstring& subKey) {
  HKEY hKey;
  std::vector<std::pair<std::wstring, RegistryValue>> values;

  if (RegOpenKeyExW(HKEY_CURRENT_USER, subKey.c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
    throw std::runtime_error("Failed to open registry key");
  }

  DWORD index = 0;
  DWORD valueNameSize, dataSize, type;
  wchar_t valueName[256];
  BYTE data[256];

  while (true) {
    valueNameSize = sizeof(valueName) / sizeof(valueName[0]);
    dataSize = sizeof(data);

    LONG result = RegEnumValueW(hKey, index, valueName, &valueNameSize, nullptr, &type, data, &dataSize);
    if (result == ERROR_NO_MORE_ITEMS) {
      break;
    }
    else if (result != ERROR_SUCCESS) {
      RegCloseKey(hKey);
      throw std::runtime_error("Failed to enumerate registry values");
    }

    std::wstring name(valueName, valueNameSize);
    if (type == REG_SZ) {
      std::wstring value(reinterpret_cast<wchar_t*>(data), dataSize / sizeof(wchar_t) - 1);
      values.emplace_back(name, value);
    }
    else if (type == REG_DWORD) {
      DWORD value = *reinterpret_cast<DWORD*>(data);
      values.emplace_back(name, value);
    }

    ++index;
  }

  RegCloseKey(hKey);
  return values;
}

RegistryValue RegEdit::GetValue(const std::wstring& valueName) const {
  auto it = std::find_if(m_values.begin(), m_values.end(), [&](const auto& pair) {
    return pair.first == valueName;
    });

  if (it != m_values.end()) {
    return it->second;
  }
  else {
    throw std::runtime_error("Value not found in loaded registry values");
  }
}

void RegEdit::SetValue(const std::wstring& valueName, const RegistryValue& value) {
  auto it = std::find_if(m_values.begin(), m_values.end(), [&](const auto& pair) {
    return pair.first == valueName;
    });

  if (it != m_values.end()) {
    it->second = value;
  }
  else {
    m_values.emplace_back(valueName, value);
  }
}

void RegEdit::Save() {
  HKEY hKey;
  if (RegOpenKeyExW(HKEY_CURRENT_USER, m_subKey.c_str(), 0, KEY_SET_VALUE, &hKey) != ERROR_SUCCESS) {
    throw std::runtime_error("Failed to open registry key");
  }

  for (const auto& [name, value] : m_values) {
    LONG result;
    if (std::holds_alternative<std::wstring>(value)) {
      const std::wstring& strValue = std::get<std::wstring>(value);
      result = RegSetValueExW(hKey, name.c_str(), 0, REG_SZ, reinterpret_cast<const BYTE*>(strValue.c_str()), (strValue.size() + 1) * sizeof(wchar_t));
    }
    else if (std::holds_alternative<DWORD>(value)) {
      DWORD dwValue = std::get<DWORD>(value);
      result = RegSetValueExW(hKey, name.c_str(), 0, REG_DWORD, reinterpret_cast<const BYTE*>(&dwValue), sizeof(dwValue));
    }
    else {
      RegCloseKey(hKey);
      throw std::runtime_error("Unsupported registry value type");
    }

    if (result != ERROR_SUCCESS) {
      RegCloseKey(hKey);
      throw std::runtime_error("Failed to set registry value");
    }
  }

  RegCloseKey(hKey);
}