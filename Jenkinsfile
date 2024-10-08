@Library('xmos_jenkins_shared_library@v0.34.0')

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
      defaultValue: 'v6.1.0',
      description: 'The xmosdoc version'
    )
  } // parameters
  options {
    skipDefaultCheckout()
    timestamps()
    buildDiscarder(xmosDiscardBuildSettings())
  } // options

  stages {
    stage('Bullds and tests') {
      parallel {
        stage('Linux builds and tests') {
          agent {
            label 'xcore.ai'
          }
          stages {
            stage('Build') {
              steps {
                runningOn(env.NODE_NAME)
                dir('lib_xcore_math') {
                  checkout scm
                  // fetch submodules
                  sh 'git submodule update --init --recursive --jobs 4'
                  withTools(params.TOOLS_VERSION) {
                    dir('examples') {
                      // xs3a build
                      sh 'cmake -B build_xs3a -G "Unix Makefiles"'
                      sh 'xmake -C build_xs3a -j'
                      // x86 build
                      sh 'cmake -B build_x86 -G "Unix Makefiles" -D BUILD_NATIVE=TRUE'
                      sh 'xmake -C build_x86 -j'
                    }
                    dir('tests/legacy_build') {
                      // legacy XCommon
                      sh 'xmake -j4'
                      sh 'xrun --io --id 0 bin/legacy_build.xe'
                      // legacy CMake
                      sh "cmake -B build --toolchain=${WORKSPACE}/lib_xcore_math/etc/xmos_cmake_toolchain/xs3a.cmake"
                      sh 'xmake -C build -j'
                      sh 'xrun --io --id 0 build/legacy_cmake_build.xe'
                    }
                  }
                }
              }
            } // Build

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
          } // stages
          post {
            cleanup {
              xcoreCleanSandbox()
            }
          }
        } // Linux builds and tests

        stage('Windows builds') {
          agent {
            label 'windows10&&unified'
          }
          stages {
            stage('Build') {
              steps {
                runningOn(env.NODE_NAME)
                dir('lib_xcore_math') {
                  checkout scm
                  // fetch submodules
                  bat 'git submodule update --init --recursive --jobs 4'
                  withTools(params.TOOLS_VERSION) {
                    withVS {
                      dir('examples') {
                        // xs3a build
                        bat 'cmake -B build_xs3a -G "Unix Makefiles"'
                        bat 'xmake -C build_xs3a'
                        // x86 build
                        bat 'cmake -B build_x86 -G Ninja -D BUILD_NATIVE=TRUE'
                        bat 'ninja -C build_x86'
                      }
                      dir('tests/legacy_build') {
                        // legacy XCommon
                        bat 'xmake --jobs 4'
                        // legacy CMake
                        bat "cmake -B build --toolchain=${WORKSPACE}/lib_xcore_math/etc/xmos_cmake_toolchain/xs3a.cmake -G Ninja"
                        bat 'ninja -C build'
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
          } // stages
          post {
            cleanup {
              xcoreCleanSandbox()
            }
          }
        } // Windows builds

        stage ('Build Documentation') {
          agent {
            label 'documentation'
          }
          stages {
            stage('Build Documentation') {
              steps {
                runningOn(env.NODE_NAME)
                dir("${REPO}") {
                  checkout scm
                }
                dir("${REPO}") {
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
