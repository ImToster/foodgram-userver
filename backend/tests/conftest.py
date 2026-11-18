import pytest
import yaml

from testsuite.databases.pgsql import discover

pytest_plugins = [
    "pytest_userver.plugins.core",
    "pytest_userver.plugins.postgresql",
]


@pytest.fixture(scope="session")
def initial_data_path(service_source_dir):
    """Path for find files with data"""
    return [
        service_source_dir / "postgresql/initial_data",
    ]


@pytest.fixture(scope="session")
def allowed_url_prefixes_extra(service_config_vars_path):
    """Allow the service to reach S3 in tests"""
    return [yaml.safe_load(service_config_vars_path.read_text())["media-url"]]


@pytest.fixture(scope="session")
def pgsql_local(service_source_dir, pgsql_local_create):
    """Create schemas databases for tests"""
    databases = discover.find_schemas(
        "test",
        [service_source_dir.joinpath("postgresql/schemas")],
    )
    return pgsql_local_create(list(databases.values()))
