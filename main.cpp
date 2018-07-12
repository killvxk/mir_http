#include <array>
#include <chrono>
#include <ctime>
#include <future>
#include <iostream>
#include <mutex>
#include <unordered_map>

#include <signal.h>

#include "http_basic.h"

#include "http_asio.h"

#include "json.h"

using namespace std::literals;

namespace neolm
{
class instance;
template <class M> class product;

class user;
class server;
class named_user_license;
class concurrent_user_license;
class named_server_license;

using instances = std::unordered_map<std::string, neolm::instance>;
using concurrent_user_licenses = std::unordered_map<std::string, neolm::product<neolm::concurrent_user_license>>;
using named_user_licenses = std::unordered_map<std::string, neolm::product<neolm::named_user_license>>;
using named_server_licenses = std::unordered_map<std::string, neolm::product<neolm::named_server_license>>;

using users = std::unordered_map<std::string, neolm::user>;
using servers = std::unordered_map<std::string, neolm::server>;

template <class M> class product
{
public:
	product(std::string id, std::string description)
		: id_(id)
		, description_(description){};

	std::string id_;
	std::string description_;

	M model_;
};

class user
{
public:
	user(std::string name)
		: name_(name){};

	std::string name_;
};

class server
{
public:
	server(std::string id, std::string hostname)
		: id_(id)
		, hostname_(hostname){};

	std::string id_;
	std::string hostname_;
};

class named_user_license
{
public:
	named_user_license() = default;
};

class named_server_license
{
public:
	named_server_license() = default;
};

class concurrent_user_license
{
public:
	concurrent_user_license() = default;
};

class instance
{
public:
	instance(json::value& instance_license, json::value& instance_allocation)
	{
		id_ = json::get<std::string>(instance_license["id"]);
		name_ = json::get<std::string>(instance_license["name"]);
		license_key_ = json::get<std::string>(instance_license["key"]);
		license_hash_ = json::get<std::string>(instance_license["bind_to"]);

		auto named_user_license_definitions = instance_license["named-user"].as_array();
		auto named_server_license_definitions = instance_license["named-server"].as_array();
		auto concurrent_user_license_definitions = instance_license["concurrent-user"].as_array();

		for (auto& named_user_license_definition : named_user_license_definitions)
		{
			named_user_licenses_.emplace(
				json::get<std::string>(named_user_license_definition["id"]),
				product<named_user_license>(
					json::get<std::string>(named_user_license_definition["id"]),
					json::get<std::string>(named_user_license_definition["description"])));
		}

		for (auto& named_server_license_definition : named_server_license_definitions)
		{
			named_server_licenses_.emplace(
				json::get<std::string>(named_server_license_definition["id"]),
				product<named_server_license>(
					json::get<std::string>(named_server_license_definition["id"]),
					json::get<std::string>(named_server_license_definition["description"])));
		}

		for (auto& concurrent_user_license_definition : concurrent_user_license_definitions)
		{
			concurrent_user_licenses_.emplace(
				json::get<std::string>(concurrent_user_license_definition["id"]),
				product<concurrent_user_license>(
					json::get<std::string>(concurrent_user_license_definition["id"]),
					json::get<std::string>(concurrent_user_license_definition["description"])));
		}

		auto users = instance_allocation["users"].as_array();
		auto servers = instance_allocation["servers"].as_array();

		for (auto& user : users)
		{
			users_.emplace(json::get<std::string>(user["name"]), json::get<std::string>(user["name"]));
		}

		for (auto& server : servers)
		{
			servers_.emplace(
				json::get<std::string>(server["name"]),
				neolm::server(json::get<std::string>(server["name"]), json::get<std::string>(server["name"])));
		}
	}

	std::string id_;
	std::string name_;
	std::string license_key_;
	std::string license_hash_;

	named_user_licenses named_user_licenses_;
	named_server_licenses named_server_licenses_;
	concurrent_user_licenses concurrent_user_licenses_;

	users users_;
	servers servers_;
};

json::value to_json(const product<concurrent_user_license>& concurrent_user_license)
{
	json::object ret;

	ret.emplace(std::string("id"), json::string(concurrent_user_license.id_));
	ret.emplace(std::string("description"), json::string(concurrent_user_license.description_));
	// product_json.emplace(std::string("id"), json::string(product.second.model_));

	return ret;
}

json::value to_json(const product<named_server_license>& name_server_license)
{
	json::object ret;

	ret.emplace(std::string("id"), json::string(name_server_license.id_));
	ret.emplace(std::string("description"), json::string(name_server_license.description_));
	// product_json.emplace(std::string("id"), json::string(product.second.model_));

	return ret;
}

json::value to_json(const product<named_user_license>& name_user_license)
{
	json::object ret;

	ret.emplace(std::string("id"), json::string(name_user_license.id_));
	ret.emplace(std::string("description"), json::string(name_user_license.description_));
	// product_json.emplace(std::string("id"), json::string(product.second.model_));

	return ret;
}

json::value to_json(const named_user_licenses& name_user_licenses)
{
	json::array ret;

	for (const auto& name_user_license : name_user_licenses)
	{
		ret.emplace_back(to_json(name_user_license.second));
	}

	return ret;
}

json::value to_json(const named_server_licenses& name_server_licenses)
{
	json::array ret;

	for (const auto& name_server_license : name_server_licenses)
	{
		ret.emplace_back(to_json(name_server_license.second));
	}

	return ret;
}

json::value to_json(const concurrent_user_licenses& concurrent_user_licenses)
{
	json::array ret;

	for (const auto& concurrent_user_license : concurrent_user_licenses)
	{
		ret.emplace_back(to_json(concurrent_user_license.second));
	}

	return ret;
}

json::value to_json(const instance& instance)
{
	json::object ret;

	ret.emplace(std::string("id"), json::string(instance.id_));
	ret.emplace(std::string("name"), json::string(instance.name_));
	ret.emplace(std::string("key"), json::string(instance.license_key_));
	ret.emplace(std::string("bind-to"), json::string(instance.license_hash_));

	json::array products_json;

	for (auto&& product : instance.named_user_licenses_)
	{
		products_json.emplace_back(to_json(product.second).as_object());
	}

	ret.emplace(std::string("named-user-licenses"), products_json);

	for (auto&& product : instance.named_server_licenses_)
	{
		products_json.emplace_back(to_json(product.second).as_object());
	}

	ret.emplace(std::string("named-server-licenses"), products_json);

	for (auto&& product : instance.concurrent_user_licenses_)
	{
		products_json.emplace_back(to_json(product.second).as_object());
	}

	ret.emplace(std::string("concurrent-user-licenses"), products_json);

	return ret;
}

json::value to_json(const instances& instances)
{
	json::array ret;

	for (const auto& instance : instances)
	{
		ret.emplace_back(to_json(instance.second).as_object());
	}

	return ret;
}

template<class S>
class license_manager
{
public:
private:
	class api_server : public S
	{
	public:
		api_server(license_manager& license_manager, http::configuration& configuration)
			: S(configuration)
			, license_manager_(license_manager)
		{
			router_.use("/static/");
			router_.use("/images/");
			router_.use("/styles/");
			router_.use("/index.html");
			router_.use("/");
			router_.use("/files/");
			// License instance configuration routes..

			router_.on_get("/null", [this](http::session_handler& session, const http::api::params& params) {
				session.response().body() = "Hoi!\n";
			});

			router_.on_get(
				"/license/:instance", [this](http::session_handler& session, const http::api::params& params) {
					if (params.get("instance").empty())
					{
						json::array return_json;

						for (auto&& instance : license_manager_.get_instances())
						{
							json::object instance_json;

							instance_json.emplace(std::string("id"), json::string(instance.second.id_));
							instance_json.emplace(std::string("name"), json::string(instance.second.name_));
							instance_json.emplace(std::string("key"), json::string(instance.second.license_key_));
							instance_json.emplace(std::string("bind-to"), json::string(instance.second.license_hash_));

							return_json.emplace_back(instance_json);
						}

						session.response().body() = json::serializer::serialize(return_json).str();
						session.response().type("json");
					}
					else
					{
						auto instance = license_manager_.get_instances().at(params.get("instance"));
						json::object return_json;

						return_json.emplace(std::string("id"), json::string(instance.id_));
						return_json.emplace(std::string("name"), json::string(instance.name_));
						return_json.emplace(std::string("key"), json::string(instance.license_key_));
						return_json.emplace(std::string("bind-to"), json::string(instance.license_hash_));

						session.response().body() = json::serializer::serialize(return_json).str();
						session.response().type("json");
					}
				});

			router_.on_post(
				"/license/:instance", [this](http::session_handler& session, const http::api::params& params) {});

			router_.on_put(
				"/license/:instance", [this](http::session_handler& session, const http::api::params& params) {});

			router_.on_delete(
				"/license/:instance", [this](http::session_handler& session, const http::api::params& params) {});

			// License model routes...
			router_.on_get(
				"/license/configuration/:instance/:license-model/:product-id/:user-name",
				[this](http::session_handler& session, const http::api::params& params) {
					const std::string& instance_id = params.get("instance");
					const std::string& license_model = params.get("license-model");
					const std::string& product_id = params.get("product-id");
					const std::string& user_name = params.get("user-name");

					if (instance_id.empty())
					{
						session.response().body()
							= json::serializer::serialize(to_json(license_manager_.get_instances())).str();
						session.response().type("json");
					}
					else if (license_model.empty())
					{
						session.response().body()
							= json::serializer::serialize(
								  to_json(license_manager_.get_instances().at(params.get(instance_id))))
								  .str();
						session.response().type("json");
					}
					else if (product_id.empty())
					{
						auto instance = license_manager_.get_instances().at(params.get("instance"));

						if (license_model == "named-users-licenses")
						{
							session.response().body()
								= json::serializer::serialize(to_json(instance.named_user_licenses_)).str();
						}
						else if (license_model == "named-server-licenses")
						{
							session.response().body()
								= json::serializer::serialize(to_json(instance.named_server_licenses_)).str();
						}
						else if (license_model == "concurrent-user-licenses")
						{
							session.response().body()
								= json::serializer::serialize(to_json(instance.concurrent_user_licenses_)).str();
						}

						session.response().type("json");
					}
					else if (user_name.empty())
					{
						auto instance = license_manager_.get_instances().at(params.get("instance"));

						if (license_model == "named-users-licenses")
						{
							session.response().body()
								= json::serializer::serialize(to_json(instance.named_user_licenses_.at(product_id)))
									  .str();
						}
						else if (license_model == "named-server-licenses")
						{
							session.response().body()
								= json::serializer::serialize(to_json(instance.named_server_licenses_.at(product_id)))
									  .str();
						}
						else if (license_model == "concurrent-user-licenses")
						{
							session.response().body() = json::serializer::serialize(
															to_json(instance.concurrent_user_licenses_.at(product_id)))
															.str();
						}

						session.response().type("json");
					}
					else
					{
						auto instance = license_manager_.get_instances().at(params.get("instance"));

						if (license_model == "named-users-licenses")
						{
							session.response().body()
								= json::serializer::serialize(to_json(instance.named_user_licenses_.at(product_id)))
									  .str();
						}
						else if (license_model == "named-server-licenses")
						{
							session.response().body()
								= json::serializer::serialize(to_json(instance.named_server_licenses_.at(product_id)))
									  .str();
						}
						else if (license_model == "concurrent-user-licenses")
						{
							session.response().body() = json::serializer::serialize(
															to_json(instance.concurrent_user_licenses_.at(product_id)))
															.str();
						}

						session.response().type("json");
					}
				});

			router_.on_post(
				"/license/:instance/model/named-user/:product-id/:user-name",
				[this](http::session_handler& session, const http::api::params& params) {});

			router_.on_put(
				"/license/:instance/model/named-user/:product-id/:user-name",
				[this](http::session_handler& session, const http::api::params& params) {});

			router_.on_delete(
				"/license/:instance/model/named-user/:product-id/:user-name",
				[this](http::session_handler& session, const http::api::params& params) {});

			// Allocation routes...
			router_.on_get(
				"/license/:instance/allocation/named-user/:product-id/:user-name",
				[this](http::session_handler& session, const http::api::params& params) {});

			router_.on_post(
				"/license/:instance/allocation/named-user/:product-id/:user-name",
				[this](http::session_handler& session, const http::api::params& params) {});

			router_.on_put(
				"/license/:instance/allocation/named-user/:product-id/:user-name",
				[this](http::session_handler& session, const http::api::params& params) {});

			router_.on_delete(
				"/license/:instance/allocation/named-user/:product-id/:user-name",
				[this](http::session_handler& session, const http::api::params& params) {});

			// Acquired routes...
			router_.on_get(
				"/license/:instance/acquired/named-user/:product-id/:user-name",
				[this](http::session_handler& session, const http::api::params& params) {});

			router_.on_post(
				"/license/:instance/acquired/named-user/:product-id/:user-name",
				[this](http::session_handler& session, const http::api::params& params) {});

			router_.on_put(
				"/license/:instance/acquired/named-user/:product-id/:user-name",
				[this](http::session_handler& session, const http::api::params& params) {});

			router_.on_delete(
				"/license/:instance/acquired/named-user/:product-id/:user-name",
				[this](http::session_handler& session, const http::api::params& params) {});

			router_.on_get("/status", [this](http::session_handler& session, const http::api::params& params) {
				server_info_.server_information(configuration_.to_string());
				server_info_.router_information(router_.to_string());
				session.response().body() = server_info_.to_string();
				session.response().type("text");
			});
		}


	private:
		friend class license_manager;
		license_manager& license_manager_;
	};

public:
public:
	license_manager(std::string home_dir)
		: configuration_{ { "server", "neolm/8.0.01" },
						  { "listen_port_begin", "3000" },
						  { "listen_port_end", "3010" },
						  { "keepalive_count", "1024" },
						  { "keepalive_timeout", "2" },
						  { "thread_count", "8" },
						  { "doc_root", "/Projects/doc_root" },
						  { "ssl_certificate", "/Projects/ssl/ssl.crt" },
						  { "ssl_certificate_key", "/Projects/ssl/ssl.key" } }
		, api_server_(*this, configuration_)
		, home_dir_(home_dir)
	{
		json::value instance_definitions = json::parser::parse(std::ifstream(home_dir_ + "instances.json"));

		for (auto& instance_definition : instance_definitions.get_array())
		{
			std::string instance_id = instance_definition["instance"].as_string();

			std::string license_file = home_dir_ + instance_id + "/license.json";
			std::string allocation_file = home_dir_ + instance_id + "/allocation.json";

			json::value instance_license = json::parser::parse(std::ifstream(license_file));
			json::value instance_allocation = json::parser::parse(std::ifstream(allocation_file));

			this->instances_.emplace(instance_id, instance(instance_license, instance_allocation));
		}
	}

	~license_manager() {}

	void start_server()
	{
		this->api_server_.start_server();
	}


	const instances& get_instances() { return instances_; }

	void add_test_routes()
	{

		for (int i = 0; i <= 1000; i++)
		{
			std::string test_route = "/key-value-store/";
			test_route += "test_";
			test_route += std::to_string(i);
			test_route += "/:key";

			api_server_.router_.on_put(
				test_route, [this, i](http::session_handler& session, const http::api::params& params) {
					auto key = std::to_string(i) + params.get("key");

					if (key.empty())
					{
						session.response().result(http::status::bad_request);
					}
					else
					{
						std::lock_guard<std::mutex> g(key_value_store_mutex_);

						try
						{
							// std::cout << key << "\n";
							key_value_store_[key] = session.request().body();
							session.response().result(http::status::created);
						}
						catch (...)
						{
							session.response().result(http::status::not_found);
						}
					}
				});

			api_server_.router_.on_get(
				test_route, [this, i](http::session_handler& session, const http::api::params& params) {
					auto key = std::to_string(i) + params.get("key");

					if (key.empty())
					{
						session.response().result(http::status::not_found);
					}
					else
					{
						std::lock_guard<std::mutex> g(key_value_store_mutex_);

						auto value = key_value_store_.find(key);

						if (value != key_value_store_.end())
						{
							session.response().body() = value->second;
						}
						else
						{
							session.response().result(http::status::not_found);
						}
					}
				});
		}
	}

private:
	std::unordered_map<std::string, std::string> key_value_store_;
	std::mutex key_value_store_mutex_;

	http::configuration configuration_;
	api_server api_server_;
	std::string home_dir_;
	instances instances_;
};

} // namespace neolm

void test_req_p_sec_simple()
{
	auto start = std::chrono::system_clock::now();

	int test_connections = 100;
	int test_requests = 1000;

	std::string readbuffer;

	readbuffer.resize(1 * 1024);

	for (int j = 0; j < test_connections; j++)
	{
		std::string url = "::1";
		network::tcp::v6 s(url, 3000);
		network::error_code ec;
		s.connect(ec);

		// auto start_requests = std::chrono::system_clock::now();

		for (int i = 0; i < test_requests; i++)
		{

			http::request_message req("GET", "/null");

			network::write(s.socket(), http::to_string(req));

			int ret = -1;
			do
			{
				ret = network::read(s.socket(), network::buffer(&readbuffer[0], readbuffer.size()));
			} while(readbuffer.find("\r\n\r\n") == std::string::npos);
		}
		// auto end_requests = std::chrono::system_clock::now();
		// std::chrono::duration<double> diff = end_requests - start_requests;

		// std::cout << j << ":" << test_requests / diff.count() << " req/sec\n";
	}

	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> diff = end - start;

	std::cout << "" << (test_connections * test_requests) << " requests took: " << diff.count() << "\n";
}

void test_post_get(size_t size, size_t nr_routes)
{

	int test_connections = 10;
	int test_requests = 1000;
	int key_index = 0;

	std::string test_body;
	static int index = 0;

	test_body.resize(size * 1024, 'A' + index++);

	auto start = std::chrono::system_clock::now();

	for (int j = 0; j < test_connections; j++)
	{
		std::string url = "::1";
		network::tcp::v6 s(url, 3000);
		network::error_code ec;
		s.connect(ec);

		auto start_requests = std::chrono::system_clock::now();

		//std::vector<char> readbuffer;

		std::string readbuffer;

		readbuffer.resize(size * 1024);

		for (int i = 0; i < test_requests; i++)
		{
			std::string test_resource
				= "/key-value-store/test_" + std::to_string(key_index) + "/key_" + std::to_string(key_index);

			// std::cout << test_resource << "\n";

			http::request_message req("PUT", test_resource);

			req.body() = test_body;
			req.content_length(req.body().length());

			network::write(s.socket(), http::to_string(req));

			int ret = -1;
			do
			{
				ret = network::read(s.socket(), network::buffer(&readbuffer[0], readbuffer.size()));
			} while(readbuffer.find("\r\n\r\n") == std::string::npos);

			key_index++;

			if (nr_routes < key_index) key_index = 0;
		}
		auto end_requests = std::chrono::system_clock::now();
		std::chrono::duration<double> diff = end_requests - start_requests;

		// std::cout << j << ":" << test_requests / diff.count() << " req/sec\n";
	}

	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> diff = end - start;

	std::cout << "" << (test_connections * test_requests) << " " << std::to_string(size)
			  << "K requests took: " << diff.count() << "\n";
}

int main(int argc, char* argv[])
{
	network::init();
	network::ssl::init();

	neolm::license_manager<http::basic::threaded::server> license_server{ "/projects/neolm_licenses/" };

	//neolm::license_manager<http::basic::async::server> license_server{ "/projects/neolm_licenses/" };


	license_server.add_test_routes();

	license_server.start_server();

	size_t size = 4;

	while (1)
	{
		std::vector<std::thread> clients;

		clients.reserve(32);


		for (int i=0; i!=4; i++)
		{
			clients.push_back(std::move(std::thread([size](){ test_post_get(size, 1000); })));
			clients.back().detach();
		}

		for (int i=0; i!=4; i++)
		{
			clients.push_back(std::move(std::thread([size](){ test_req_p_sec_simple(); })));
			clients.back().detach();
		}


		size = size;
		if (size > 32)
			size = 4;

		std::this_thread::sleep_for(10s);
	}
}
