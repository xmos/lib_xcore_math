@Library('xmos_jenkins_shared_library@v0.45.0')

def runningOn(machine) {
  println "Stage running on:"
  println machine
}

getApproval()
pipeline {
  agent none
    environment {
    REPO = 'lib_xcore_math'
  }
  parameters {
    string(
      name: 'TOOLS_VERSION',
      defaultValue: '15.3.0',
      description: 'The XTC tools version'
    )
    booleanParam(
      name: 'XMATH_SMOKE_TEST',
      defaultValue: true,
      description: 'Enable smoke run'
    )
    string(
      name: 'XMOSDOC_VERSION',
      defaultValue: 'v8.0.1',
      description: 'The xmosdoc version'
    )
    string(
        name: 'INFR_APPS_VERSION',
        defaultValue: 'v3.3.0',
        description: 'The infr_apps version'
    )
  } // parameters
  options {
    skipDefaultCheckout()
    timestamps()
    buildDiscarder(xmosDiscardBuildSettings())
  } // options

  stages {
    stage('Build and test') {
      parallel {
        stage('Linux build & test') {
          agent {
            label 'xcore.ai'
          }
          stages {

            stage('Examples') {
              steps {
                runningOn(env.NODE_NAME)
                dir("${REPO}") {
                  checkout scm
                  withTools(params.TOOLS_VERSION) {
                    dir('examples') {
                      // xs3a build
                      sh 'cmake -B build_xs3a -G "Unix Makefiles"'
                      sh 'xmake -C build_xs3a -j'
                      // x86 build
                      sh 'cmake -B build_x86 -G "Unix Makefiles" -D BUILD_NATIVE=TRUE'
                      sh 'xmake -C build_x86 -j'
                    }
                  }
                }
              }
            } // Build examples

            stage('Unit tests xs3a') {
              steps {
                withTools(params.TOOLS_VERSION) {
                  dir('lib_xcore_math/tests') {
                    sh "cmake -B build_xs3a  -DXMATH_SMOKE_TEST=${params.XMATH_SMOKE_TEST} -G \"Unix Makefiles\""
                    sh 'xmake -C build_xs3a -j'

                    sh 'xrun --xscope --id 0 --args bfp_tests/bin/bfp_tests.xe        -v'
                    sh 'xrun --xscope --id 0 --args dct_tests/bin/dct_tests.xe        -v'
                    sh 'xrun --xscope --id 0 --args fft_tests/bin/fft_tests.xe        -v'
                    sh 'xrun --xscope --id 0 --args filter_tests/bin/filter_tests.xe  -v'
                    sh 'xrun --xscope --id 0 --args scalar_tests/bin/scalar_tests.xe  -v'
                    sh 'xrun --xscope --id 0 --args vect_tests/bin/vect_tests.xe      -v'
                    sh 'xrun --xscope --id 0 --args xs3_tests/bin/xs3_tests.xe        -v'
                  }
                }
              }
            } // Unit tests xs3a

            stage('Unit tests x86') {
              steps {
                withTools(params.TOOLS_VERSION) {
                  dir('lib_xcore_math/tests') {
                    sh "cmake -B build_x86 -DXMATH_SMOKE_TEST=${params.XMATH_SMOKE_TEST} -G \"Unix Makefiles\" -D BUILD_NATIVE=TRUE"
                    sh 'xmake -C build_x86 -j'

                    sh './bfp_tests/bin/bfp_tests        -v'
                    sh './dct_tests/bin/dct_tests        -v'
                    sh './fft_tests/bin/fft_tests        -v'
                    sh './filter_tests/bin/filter_tests  -v'
                    sh './scalar_tests/bin/scalar_tests  -v'
                    sh './vect_tests/bin/vect_tests      -v'
                    sh './xs3_tests/bin/xs3_tests        -v'
                  }
                }
              }
            } // Unit tests x86

            stage('Library checks') {
                steps {
                    runRepoChecks("${WORKSPACE}/${REPO}")
                }
            }

            stage('Legacy build') {
              steps {
                runningOn(env.NODE_NAME)
                sh 'git clone --branch=v1.0.0 git@github.com:xmos/xmos_cmake_toolchain'
                dir("${REPO}") {
                  checkout scm
                  withTools(params.TOOLS_VERSION) {
                    dir('tests/legacy_build') {
                      // legacy CMake
                      sh "cmake -B build --toolchain=${WORKSPACE}/xmos_cmake_toolchain/xs3a.cmake"
                      sh 'xmake -C build -j'
                      sh 'xrun --io --id 0 build/legacy_cmake_build.xe'
                    }
                  }
                }
              }
            } // Legacy build
          } // stages
          post {
            cleanup {
              xcoreCleanSandbox()
            }
          }
        } // Linux build and test

        stage('Windows build & test') {
          agent {
            label 'windows10&&unified'
          }
          stages {
            stage('Examples') {
              steps {
                runningOn(env.NODE_NAME)
                dir('lib_xcore_math') {
                  checkout scm
                  withTools(params.TOOLS_VERSION) {
                    dir('examples') {
                      // xs3a build
                      bat 'cmake -B build_xs3a -G "Unix Makefiles"'
                      bat 'xmake -C build_xs3a'
                      withVS {
                        // x86 build
                        bat 'cmake -B build_x86 -G Ninja -D BUILD_NATIVE=TRUE'
                        bat 'ninja -C build_x86'
                      }
                    }
                  }
                }
              }
            } // Build

            stage('Unit tests x86') {
              steps {
                dir('lib_xcore_math/tests') {
                  withTools(params.TOOLS_VERSION) {
                    withVS {
                      bat 'cmake -B build_x86 -DXMATH_SMOKE_TEST=${params.XMATH_SMOKE_TEST} -G Ninja -D BUILD_NATIVE=TRUE'
                      bat 'ninja -C build_x86'

                      bat 'bfp_tests\\bin\\bfp_tests.exe        -v'
                      bat 'dct_tests\\bin\\dct_tests.exe        -v'
                      bat 'fft_tests\\bin\\fft_tests.exe        -v'
                      bat 'filter_tests\\bin\\filter_tests.exe  -v'
                      bat 'scalar_tests\\bin\\scalar_tests.exe  -v'
                      bat 'vect_tests\\bin\\vect_tests.exe      -v'
                      bat 'xs3_tests\\bin\\xs3_tests.exe        -v'
                    }
                  }
                }
              }
            } // Unit tests x86

            stage('Legacy build') {
              steps {
                runningOn(env.NODE_NAME)
                bat 'git clone --branch=v1.0.0 git@github.com:xmos/xmos_cmake_toolchain'
                dir('lib_xcore_math') {
                  checkout scm
                  withTools(params.TOOLS_VERSION) {
                    withVS {
                      dir('tests/legacy_build') {
                        // legacy CMake
                        bat "cmake -B build --toolchain=${WORKSPACE}/xmos_cmake_toolchain/xs3a.cmake -G Ninja"
                        bat 'ninja -C build'
                      }
                    }
                  }
                }
              }
            } // Legacy build

          } // stages
          post {
            cleanup {
              xcoreCleanSandbox()
            }
          }
        } // Windows builds

        stage ('Build documentation') {
          agent {
            label 'documentation'
          }
          stages {
            stage('Documentation') {
              steps {
                runningOn(env.NODE_NAME)
                dir("${REPO}") {
                  checkout scm
                  warnError("Docs") {
                    buildDocs()
                  } // warnError("Docs")
                } // dir("${REPO}")
              } // steps
            } // stage('Build Documentation')
          } // stages
          post {
            cleanup {
              xcoreCleanSandbox()
            }
          }
        } // Build Documentation

      } // parallel
    } // Bullds and tests
  } // stages
} // pipeline
