message(STATUS "downloading...
     src='https://bitbucket.org/transporter/ogre-procedural/get/ca6eb3363a53.zip'
     dst='/home/matheus/cycleshooter/vendor/ogre-procedural/src/ca6eb3363a53.zip'
     timeout='none'")




file(DOWNLOAD
  "https://bitbucket.org/transporter/ogre-procedural/get/ca6eb3363a53.zip"
  "/home/matheus/cycleshooter/vendor/ogre-procedural/src/ca6eb3363a53.zip"
  SHOW_PROGRESS
  # no EXPECTED_HASH
  # no TIMEOUT
  STATUS status
  LOG log)

list(GET status 0 status_code)
list(GET status 1 status_string)

if(NOT status_code EQUAL 0)
  message(FATAL_ERROR "error: downloading 'https://bitbucket.org/transporter/ogre-procedural/get/ca6eb3363a53.zip' failed
  status_code: ${status_code}
  status_string: ${status_string}
  log: ${log}
")
endif()

message(STATUS "downloading... done")
