@Library("JenkinsPipelines") _

/*
 * This pipeline uses the 'dockerComposePipeline' to manage the deployment of this Docker Compose application.
 *
 * Configuration:
 * - defaultBitwardenEnabled: true
 * Enables Bitwarden integration by default, pulling a .env file from a secure note with the same name as this repository.
 *
 * - disableTriggers: true
 * Disables automatic builds from webhooks or SCM scans. This job will only run when manually triggered.
 *
 * Requirements:
 * - JenkinsPipelines Library: https://github.com/mwdle/JenkinsPipelines
 * - JenkinsBitwardenUtils Library (for Bitwarden integration): https://github.com/mwdle/JenkinsBitwardenUtils
 */
dockerComposePipeline(defaultBitwardenEnabled: true, disableTriggers: true)