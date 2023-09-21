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
  } // parameters
  options {
    skipDefaultCheckout()
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
                    sh 'cmake -B build_xs3a -DXMATH_MATH_SMOKE_TEST=ON --toolchain=etc/xmos_cmake_toolchain/xs3a.cmake'
                    sh 'make -C build_xs3a -j4'
                    // x86 build
                    sh 'cmake -B build_x86 -DXMATH_MATH_SMOKE_TEST=ON'
                    sh 'make -C build_x86 -j4'
                  }
                }
              }
            } // Build

            stage('Unit tests') {
              steps {
                dir('build_xs3a/test') {
                  withTools(params.TOOLS_VERSION) {
                    sh 'xrun --xscope bfp_tests/bfp_tests.xe        -v'
                    sh 'xrun --xscope dct_tests/dct_tests.xe        -v'
                    sh 'xrun --xscope fft_tests/fft_tests.xe        -v'
                    sh 'xrun --xscope filter_tests/filter_tests.xe  -v'
                    sh 'xrun --xscope scalar_tests/scalar_tests.xe  -v'
                    sh 'xrun --xscope vect_tests/vect_tests.xe      -v'
                    sh 'xrun --xscope xs3_tests/xs3_tests.xe        -v'
                  }
                }
                dir('build_x86/test') {
                  sh './bfp_tests/bfp_tests        -v'
                  sh './dct_tests/dct_tests        -v'
                  sh './fft_tests/fft_tests        -v'
                  sh './filter_tests/filter_tests  -v'
                  sh './scalar_tests/scalar_tests  -v'
                  sh './vect_tests/vect_tests      -v'
                }
              }
            } // Unit tests
          } // stages
          post {
            cleanup {
              cleanWs()
            }
          }
        } // Linux builds and tests

        stage ('Build Documentation') {
          agent {
            label 'docker'
          }
          stages {
            stage('Build Docs') {
              steps {
                runningOn(env.NODE_NAME)
                checkout scm
                sh """docker run --user "\$(id -u):\$(id -g)" \
                        --rm \
                        -v ${WORKSPACE}:/build \
                        -e EXCLUDE_PATTERNS="/build/doc/doc_excludes.txt" \
                        -e DOXYGEN=1 -e DOXYGEN_INCLUDE=/build/doc/Doxyfile.in \
                        -e PDF=1 \
                        ghcr.io/xmos/doc_builder:v3.0.0"""
                
                archiveArtifacts artifacts: "doc/_build/**", allowEmptyArchive: true
              } // steps
            } // Build Docs
          } // stages
          post {
            cleanup {
              cleanWs()
            }
          }
        } // Build Documentation

      } // parallel
    } // Bullds and tests
  } // stages
} // pipeline