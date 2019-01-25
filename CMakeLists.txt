hyro_init_package(hyro_signal_generator VERSION 0.0.1)

include ( FindPkgConfig  )
find_package ( Qt4 COMPONENTS QtCore QtGui QtOpenGl )
find_package ( QWT )

hyro_find_package(hyro_utils REQUIRED)
hyro_find_package(hyro_msgs REQUIRED)
hyro_find_package(hyro_widgets REQUIRED)

include(${QT_USE_FILE} )
add_definitions(${QT_DEFINITIONS})

include_directories(
  ${PROJECT_SOURCE_DIR}/src
)

QT4_ADD_RESOURCES(QtApp_RCC_SRCS src/widgets/resources.qrc)
qt4_wrap_cpp( ${PROJECT_NAME}_MOC_CPP ${hyro_widgets_HEADERS_MOC} )
  
set(SIGNAL_GENERATOR_HEADERS
  include/hyro/DigitalConverterComponent.h
  include/hyro/SignalGeneratorComponent.h
  include/hyro/SignalGenerator.h
  include/hyro/Thresholding.h
  include/hyro/msgs/Signal.h
  include/hyro/msgs/Signal.proto.h
)

set(SIGNAL_GENERATOR_SOURCES
  src/components/DigitalConverterComponent.cpp
  src/components/SignalGeneratorComponent.cpp
  src/internal/SignalGenerator.cpp
  src/internal/Thresholding.cpp
)

hyro_add_library(
  ${PROJECT_NAME}_components
  HEADERS ${SIGNAL_GENERATOR_HEADERS}
  SOURCES ${SIGNAL_GENERATOR_SOURCES}
  DEPENDS_ON
    hyro_utils
    hyro_msgs
    hyro_widgets
)

hyro_add_executable(${PROJECT_NAME}
  SOURCES
    src/examples/signal_generator.cpp
  DEPENDS_ON
    ${PROJECT_NAME}_components
)

hyro_install_package(${PROJECT_NAME}
  EXES
    ${PROJECT_NAME}
)

hyro_compile_messages(
  ${PROJECT_NAME}_components SHARED
  SOURCES
    msgs/hyro/msgs/Signal.proto
)

hyro_add_gtest(${PROJECT_NAME}
  SOURCES
    tests/test_signal_generator.cpp
  DEPENDS_ON
    ${PROJECT_NAME}_components
)