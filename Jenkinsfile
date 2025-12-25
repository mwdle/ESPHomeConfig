@Library("JenkinsPipelines") _

/*
 * This Docker Compose deployment is managed by the `dockerComposePipeline` defined in the
 * Jenkins Pipelines shared library (https://github.com/mwdle/JenkinsPipelines).
 *
 * Configuration:
 * - envFileCredentialIds:
 * Injects secrets from a Jenkins 'Secret file' credential. It expects the credential ID 
 * to match the name of this repository, suffixed with '.env'.
 *
 * - persistentWorkspace:
 * Deploys to a stable directory on the host to preserve data between builds. The path is
 * dynamically set using the DOCKER_VOLUMES environment variable.
 *
 * - disableTriggers: true
 * Disables automatic builds from webhooks or SCM scans. This job will only run when
 * manually triggered.
 *
 * - postCheckoutSteps:
 * Injects the secrets.yaml file from Jenkins credentials into the config directory.
 * This file contains WiFi credentials, API keys, and OTA passwords required by ESPHome
 * during device compilation. The file is copied from the Jenkins credential store
 * (credential ID: 'secrets.yaml') with restricted permissions (600).
 */
dockerComposePipeline(
    envFileCredentialIds: ["ESPHomeConfig.env"],
    persistentWorkspace: "/home/mwdle/docker/deployments",
    disableTriggers: true,
    postCheckoutSteps: {
        stage('Inject ESPHome Secrets') {
            withCredentials([file(credentialsId: 'secrets.yaml', variable: 'SECRETS_FILE')]) {
                sh '''
                    cp $SECRETS_FILE config/secrets.yaml
                    chmod 600 config/secrets.yaml
                '''
            }
        }
    }
)