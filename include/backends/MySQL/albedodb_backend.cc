#include "albedodb_backend.h"

namespace Albedo {
namespace DB {

	Query::Query(bool store, Backend& host, std::string_view _SQL)
		:host{ host }
	{
		host._check_error(mysql_real_query(host.m_MySQL, _SQL.data(), _SQL.size()));
		if (store)
		{
			m_result = mysql_store_result(host.m_MySQL);
			if (!m_result) throw std::runtime_error("Failed to store result");
		}
		else
		{
			m_result = mysql_use_result(host.m_MySQL);
			if (!m_result) throw std::runtime_error("Failed to use result");
		}
	}

	Query::~Query()
	{
		mysql_free_result(m_result);
	}

    
        
}} // namespace Albedo::DB