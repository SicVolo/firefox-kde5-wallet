cmake_minimum_required(VERSION 2.8.12)

project(firefox-kde5-wallet)

find_package(ECM REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings)
include(KDECMakeSettings)
include(ECMMarkAsTest)

include(FeatureSummary)

find_package(KF5Wallet 5.3 REQUIRED)

find_package(Qt5 5.2 CONFIG REQUIRED Core)

set(firefox-kde5-wallet_SRCS
    KDE5Wallet.cpp
)

add_library(libkde5wallet MODULE ${firefox-kde5-wallet_SRCS})

target_link_libraries(libkde5wallet
PUBLIC
    KF5::Wallet
    Qt5::Core
)

add_executable(testkde5wallet testKDE5Wallet.c)
target_link_libraries(testkde5wallet KF5::Wallet ${CMAKE_DL_LIBS})
ecm_mark_as_test(testkde5wallet)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
