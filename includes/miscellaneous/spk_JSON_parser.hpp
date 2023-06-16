#pragma once

namespace spk
{
    class JSONParser
    {
    private:

    public:
        JSONParser()
        {

        }

        void load(const std::wstring& p_path)
        {
            std::fstream file;
            file.open(p_path.c_str(), std::ios_base::in);

            
        }
    };
}