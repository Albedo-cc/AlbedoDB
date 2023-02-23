#pragma once

#include <albedodb_backend.h>

#include <memory>
#include <string>
#include <cassert>

namespace Albedo {
namespace DB {

	class Database // Singleton
	{
	public:
        Database(std::string_view host,
                std::string_view user,
                std::string_view password,
                std::string_view database,
                uint32_t port = ALBEDODB_DEFAULT_PORT) :
            m_database{ std::make_unique<Backend>() }
        {
            m_database->log_in(host, user, password, database, port);
        }
        ~Database() noexcept = default;

    public:
        std::shared_ptr<Query> query(std::string_view _SQL_select, bool bStore = true)
        {
            if (bStore) return m_database->select_and_store(_SQL_select);
            else return m_database->select_and_use(_SQL_select);
        }

        void command(std::string_view _SQL)
        {
            m_database->execute_SQL(_SQL);
        }

        Database(const Database&) = delete;
        Database(Database&&) = delete;
        Database& operator=(const Database&) = delete;
        Database& operator=(Database&&) = delete;

    private:
        std::unique_ptr<Backend> m_database;
	};

}} // namespace Albedo::DB