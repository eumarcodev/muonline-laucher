#pragma once

using RegistryValue = std::variant<std::wstring, DWORD>;
using RegistryStruct = std::vector<std::pair<std::wstring, RegistryValue>>;

class RegEdit {
public:

  RegEdit();
  ~RegEdit();
  void Load();
  void Save();
  RegistryStruct& GetValues() const;
  RegistryValue GetValue(const std::wstring& name) const;
  void SetValue(const std::wstring& name, const RegistryValue& value);
  
private:
  RegistryStruct getAllRegistryValues(const std::wstring& subKey);
  mutable RegistryStruct m_values;
  const std::wstring m_subKey = L"Software\\Webzen\\Mu\\Config";  
};