#include "workload.h"

Workload::Workload(const char *name, int flops, int data, int nvec)
{
    m_flops = flops;
    m_bytes = data;
    m_name = name;
    m_nvec = nvec;
}

void Workload::init()
{
    m_ints.resize(m_nvec);
    m_longs.resize(m_nvec);
    m_floats.resize(m_nvec);
    m_doubles.resize(m_nvec);

    for (int i = 0; i < m_nvec; i++) {
        m_ints[i].fill(0, m_size);
        m_longs[i].fill(0, m_size);
        m_floats[i].fill(0, m_size);
        m_doubles[i].fill(0, m_size);
    }
}

void Workload::teardown()
{
    m_ints.resize(0);
    m_longs.resize(0);
    m_floats.resize(0);
    m_doubles.resize(0);
}
