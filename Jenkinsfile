@Library('xmos_jenkins_shared_library@v0.27.0')

def runningOn(machine) {
  println "Stage running on:"
  println machine
}

getApproval()
pipeline {
  agent none

  parameters {
    string(
      name: 'TOOLS_VERSION',
      defaultValue: '15.2.1',
      description: 'The XTC tools version'
    )
    booleanParam(
      name: 'XMATH_SMOKE_TEST',
      defaultValue: true,
      description: 'Enable smoke run'
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
                    // xs3a build
                    sh "cmake -B build_xs3a -DXMATH_SMOKE_TEST=${params.XMATH_SMOKE_TEST} --toolchain=etc/xmos_cmake_toolchain/xs3a.cmake"
                    sh 'make -C build_xs3a -j4'
                    // x86 build
                    sh "cmake -B build_x86 -DXMATH_SMOKE_TEST=${params.XMATH_SMOKE_TEST}"
                    sh 'make -C build_x86 -j4'
                    // xmake build
                    dir('test/legacy_build') {
                      sh 'xmake -j4'
                      sh 'xrun --io --id 0 bin/legacy_build.xe'
                    }
                  }
                }
              }
            } // Build

            stage('Unit tests xs3a') {
              steps {
                dir('lib_xcore_math/build_xs3a/test') {
                  withTools(params.TOOLS_VERSION) {
                    sh 'xrun --xscope --id 0 --args bfp_tests/bfp_tests.xe        -v'
                    sh 'xrun --xscope --id 0 --args dct_tests/dct_tests.xe        -v'
                    sh 'xrun --xscope --id 0 --args fft_tests/fft_tests.xe        -v'
                    sh 'xrun --xscope --id 0 --args filter_tests/filter_tests.xe  -v'
                    sh 'xrun --xscope --id 0 --args scalar_tests/scalar_tests.xe  -v'
                    sh 'xrun --xscope --id 0 --args vect_tests/vect_tests.xe      -v'
                    sh 'xrun --xscope --id 0 --args xs3_tests/xs3_tests.xe        -v'
                  }
                }
              }
            } // Unit tests xs3a

            stage('Unit tests x86') {
              steps {
                dir('lib_xcore_math/build_x86/test') {
                  sh './bfp_tests/bfp_tests        -v'
                  sh './dct_tests/dct_tests        -v'
                  sh './fft_tests/fft_tests        -v'
                  sh './filter_tests/filter_tests  -v'
                  sh './scalar_tests/scalar_tests  -v'
                  sh './vect_tests/vect_tests      -v'
                  sh './xs3_tests/xs3_tests        -v'
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
                      // xs3a build
                      bat "cmake -B build_xs3a -DXMATH_SMOKE_TEST=${params.XMATH_SMOKE_TEST} --toolchain=etc/xmos_cmake_toolchain/xs3a.cmake -G Ninja"
                      bat "ninja -C build_xs3a"
                      // x86 build
                      bat "cmake -B build_x86 -DXMATH_SMOKE_TEST=${params.XMATH_SMOKE_TEST} -G Ninja"
                      bat "ninja -C build_x86"
                      // xmake build
                      dir('test/legacy_build') {
                        bat 'xmake --jobs 4'
                      }
                    }
                  }
                }
              }
            } // Build

            stage('Unit tests x86') {
              steps {
                dir('lib_xcore_math/build_x86/test') {
                  bat 'bfp_tests\\bfp_tests.exe        -v'
                  bat 'dct_tests\\dct_tests.exe        -v'
                  bat 'fft_tests\\fft_tests.exe        -v'
                  bat 'filter_tests\\filter_tests.exe  -v'
                  bat 'scalar_tests\\scalar_tests.exe  -v'
                  bat 'vect_tests\\vect_tests.exe      -v'
                  bat 'xs3_tests\\xs3_tests.exe        -v'
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
            label 'docker'
          }
          environment {
            XMOSDOC_VERSION = "v4.0"
          }
          stages {
            stage('Build Docs') {
              steps {
                runningOn(env.NODE_NAME)
                checkout scm
                script {
                  def settings = readYaml file: 'settings.yml'
                  def doc_version = settings["version"]
                
                  sh "docker pull ghcr.io/xmos/xmosdoc:$XMOSDOC_VERSION"
                  sh """docker run -u "\$(id -u):\$(id -g)" \
                      --rm \
                      -v ${WORKSPACE}:/build \
                      ghcr.io/xmos/xmosdoc:$XMOSDOC_VERSION -v"""
                    zip zipFile: "docs_xcore_math_v${doc_version}.zip", archive: true, dir: "doc/_build"
                }
              } // steps
            } // Build Docs
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
