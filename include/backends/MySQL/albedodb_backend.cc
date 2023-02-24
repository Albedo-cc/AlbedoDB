#include "albedodb_backend.h"

namespace Albedo {
namespace DB {

	Query::Query(bool store, Backend& host, std::string_view _SQL):
		host{ host }
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

	std::shared_ptr<Query::Result> Query::get_result() const
	{
		std::shared_ptr<Result> result = std::make_shared<Result>();
		result->m_result.reserve(m_result->row_count);

		std::vector<Result::Field> fields(m_result->field_count);
		// Initialize Fields
		for (auto& field : fields) field = mysql_fetch_field(m_result)->name;

		MYSQL_ROW data = nullptr;
		for (size_t row = 0; data = mysql_fetch_row(m_result); ++row)
		{
			auto& kv = result->m_result.emplace_back();
			for (size_t col = 0; col < fields.size(); ++col)
			{
				kv.insert({ fields[col], data[col] });
			}
		}
		return result;
	}

	Query::Result::Cursor Query::Result::search_unique(std::string_view field, std::string_view unique_value)
	{
		if (m_result.empty() || field.empty() || // Empty
			(m_result[0].find(field.data()) == m_result[0].end())) // Non-existing field
			return { NULL };

		for (size_t row = 0; row < m_result.size(); ++row)
		{
			if (m_result[row][field.data()] == unique_value) return { row + 1 };
		}
			
		return { NULL };
	}

	Query::Result::Item Query::Result::fetch(Cursor cursor)
	{
		if (!cursor || cursor > m_result.size()) return {};
		else return m_result[cursor - 1];
	}

}} // namespace Albedo::DB