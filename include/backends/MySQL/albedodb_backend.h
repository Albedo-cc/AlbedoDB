#pragma once

#include <mysql.h>

#include <stdexcept>
#include <string>
#include <string_view>
#include <memory>

namespace Albedo {
namespace DB
{
	const uint32_t ALBEDODB_DEFAULT_PORT = 3306;

	class Backend;

	class Query
	{
	public:
		inline MYSQL_RES* const get() const { return m_result; }

		Query() = delete;
		Query(bool store, Backend& host, std::string_view _SQL);
		~Query();
	private:
		Backend& host;
		MYSQL_RES* m_result = nullptr;
	};

	class Backend
	{
		friend class Query;
	public:
		// Interface
		 // If not store result you have to use mysql_fecth_row() to get result
		std::shared_ptr<Query> select_and_store(std::string_view _SQL)
		{
			return std::make_shared<Query>(true, *this, _SQL);
		}

		std::shared_ptr<Query> select_and_use(std::string_view _SQL)
		{
			return std::make_shared<Query>(false, *this, _SQL);
		}

		void execute_SQL(std::string_view _SQL)
		{
			_check_error(mysql_real_query(m_MySQL, _SQL.data(), _SQL.size()));
		}

		const char* ping()
		{
			if (mysql_ping(m_MySQL)) return mysql_error(m_MySQL);
			else return "Ping success!";
		}

		void set_option(mysql_option option, const void* arg)
		{
			_check_error(mysql_options(m_MySQL, option, arg));
		}

		void log_in(std::string_view host,
							std::string_view user,
							std::string_view password,
							std::string_view database,
							uint32_t port = ALBEDODB_DEFAULT_PORT)
		{
			mysql_real_connect(m_MySQL,
				host.data(), 
				user.data(),
				password.data(),
				database.data(),
				port,
				NULL,	//unix_socket
				NULL	// client flag
			);
		}

	protected:
		MYSQL* m_MySQL = nullptr;

		uint8_t opt_connect_timeout = 5;
		bool		 opt_reconnect = true;
	public:
		//Backend() = delete;
		Backend()
		{	
			m_MySQL = mysql_init(nullptr);

			set_option(MYSQL_OPT_CONNECT_TIMEOUT, &opt_connect_timeout);
			set_option(MYSQL_OPT_RECONNECT, &opt_reconnect);
		}

		~Backend()
		{
			mysql_close(m_MySQL);
			mysql_server_end();
		}

		Backend(const Backend&) = delete;
		Backend& operator = (const Backend&) = delete;

	private:
		inline void _check_error(int res)
		{
			if (res) throw std::runtime_error(mysql_error(m_MySQL));
		}
	};

}} // namespace Albedo::DB