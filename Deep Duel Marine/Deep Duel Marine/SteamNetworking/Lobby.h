#pragma once

struct Lobby {
    unsigned long long m_id = 0;
    char m_name[64]{};
    bool m_isPublic = true;
};
