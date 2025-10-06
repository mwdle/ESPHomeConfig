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
 */
dockerComposePipeline(envFileCredentialIds: [env.JOB_NAME.split('/')[1] + ".env"], persistentWorkspace: "${env.DOCKER_VOLUMES}/deployments", disableTriggers: true)