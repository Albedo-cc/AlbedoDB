
#pragma once

#include <mysql.h>

#include <stdexcept>
#include <string>
#include <string_view>

namespace Albedo {
namespace DB
{

	class Backend
	{
		class Query
		{
			friend class Backend;
		public:
			inline MYSQL_RES* const get() const { return m_result; }

			Query() = delete;
			Query(bool store, Backend& host, std::string_view _SQL)
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
			~Query()
			{
				mysql_free_result(m_result);
			}
		private:
			Backend& host;
			MYSQL_RES* m_result = nullptr;
		};

	public:
		// Interface
		inline std::shared_ptr<Query> query(std::string_view _SQL_select, bool store) // If not store result you have to use mysql_fecth_row() to get result
		{
			return std::make_shared<Query>(store, *this, _SQL_select);
		}

		inline void exec(std::string_view _SQL)
		{
			_check_error(mysql_real_query(m_MySQL, _SQL.data(), _SQL.size()));
		}

		inline const char* ping()
		{
			if (mysql_ping(m_MySQL)) return mysql_error(m_MySQL);
			else return "Ping success!";
		}

	protected:
		MYSQL* m_MySQL = nullptr;
		
	public:
		Backend() = delete;
		Backend()
		{
			m_MySQL = mysql_init(nullptr);
		}

		~Backend()
		{
			mysql_close(m_MySQL);
			mysql_server_end();
		}

		Backend(const Database&) = delete;
		Backend& operator = (const Database&) = delete;

		inline void _set_option(mysql_option option, const void* arg)
		{
			_check_error(mysql_options(m_MySQL, option, arg));
		}

		inline void _check_error(int res)
		{
			if (res) throw std::runtime_error(mysql_error(m_MySQL));
		}
	};

}} // namespace Albedo::DB