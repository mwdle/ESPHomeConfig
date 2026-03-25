@Library("JenkinsPipelines") _ // See https://github.com/mwdle/JenkinsPipelines

dockerComposePipeline(
    disableConcurrentBuilds: true,
    envFileCredentialIds: ["ESPHomeConfig.env"],
    persistentWorkspace: "${env.DOCKER_VOLUMES}/deployments",
    postCheckoutSteps: {
        stage('Inject ESPHome Secrets') {
            withCredentials([file(credentialsId: 'ESPHome secrets.yaml', variable: 'SECRETS_FILE')]) {
                sh '''
                    cp "$SECRETS_FILE" config/secrets.yaml
                    chmod 400 config/secrets.yaml
                '''
            }
        }
    }
)
