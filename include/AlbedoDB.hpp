#pragma once

#include <albedodb_backend.h>

#include <memory>

namespace Albedo {
namespace DB {

    class Query;

	class Database // Singleton
	{
	private:
        Database() :
            m_database{ std::make_unique<Backend>() }
        {

        }
        ~Database() noexcept = default;
    public:
        static Database& instance()
        {
            static Database singleton;
            return singleton;
        }

        inline std::shared_ptr<Query> query(std::string_view _SQL_select, bool store) // If not store result you have to use mysql_fecth_row() to get result
        {
            return std::make_shared<Query>(store, *this, _SQL_select);
        }

        inline void exec(std::string_view _SQL)
        {
            m_database->exec(_SQL);
        }

        Database(const Database&) = delete;
        Database(Database&&) = delete;
        Database& operator=(const Database&) = delete;
        Database& operator=(Database&&) = delete;

    private:
        std::unique_ptr<Backend> m_database;
	};

}} // namespace Albedo::DB