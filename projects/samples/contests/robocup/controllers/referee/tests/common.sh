assert_env_vars() {
  if [ -z "${WEBOTS_HOME+x}" ]; then
    >&2 echo "Error: WEBOTS_HOME is not set"
    exit 1
  fi

  if [ -z "${JAVA_HOME+x}" ]; then
    >&2 echo "Error: JAVA_HOME is not set"
    exit 1
  fi

  if [ -z "${GAME_CONTROLLER_HOME+x}" ]; then
    >&2 echo "Error: GAME_CONTROLLER_HOME is not set"
    exit 1
  fi
}
