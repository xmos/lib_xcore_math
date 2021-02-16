@Library('xmos_jenkins_shared_library@v0.14.2') _

getApproval()

pipeline {
    agent {
        dockerfile {
            args "-v /etc/passwd:/etc/passwd:ro -v /etc/group:/etc/group:ro -v /home/jenkins/.ssh:/home/jenkins/.ssh:ro"
        }
    }

    parameters { // Available to modify on the job page within Jenkins if starting a build
        string( // use to try different tools versions
            name: 'TOOLS_VERSION',
            defaultValue: '15.0.4',
            description: 'The tools version to build with (check /projects/tools/ReleasesTools/)'
        )
    }

    options { // plenty of things could go here
        //buildDiscarder(logRotator(numToKeepStr: '10'))
        timestamps()
    }

    stages {
        stage("Setup") {
            // Clone and install build dependencies
            steps {
                // clean auto default checkout
                sh "rm -rf *"
                // clone
                checkout([
                    $class: 'GitSCM',
                    branches: scm.branches,
                    doGenerateSubmoduleConfigurations: false,
                    extensions: [[$class: 'CleanCheckout']],
                    userRemoteConfigs: [[credentialsId: 'xmos-bot',
                                         url: 'git@github.com:xmos/lib_xs3_math']]
                ])
                // fetch dependencies
                sshagent (credentials: ['xmos-bot']) {
                    dir("${env.WORKSPACE}/test") {
                        sh "git clone git@github.com:xmos/Unity.git"
                        //sh "python fetch_dependencies.py"
                    }                
                }
                // create venv
                sh "conda env create -q -p lib_xs3_math_venv -f environment.yml"
                // install xmos tools version
                sh "/XMOS/get_tools.py " + params.TOOLS_VERSION
            }
        }
        stage("Update all packages") {
            // Roll all conda packages forward beyond their pinned versions
            when { expression { return params.UPDATE_ALL } }
            steps {
                sh "conda update --all -y -q -p lib_xs3_math_venv"
            }
        }
        stage("Build") {
            steps {
                // below is how we can activate the tools
                sh """pushd /XMOS/tools/${params.TOOLS_VERSION}/XMOS/xTIMEcomposer/${params.TOOLS_VERSION} && . SetEnv && popd &&
                      . activate ./lib_xs3_math_venv &&
                      cd test && make all"""
            }
        }
    }
    post {
        cleanup {
            cleanWs()
        }
    }
}
