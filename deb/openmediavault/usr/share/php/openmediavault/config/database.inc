<?php
/**
 * This file is part of OpenMediaVault.
 *
 * @license   http://www.gnu.org/licenses/gpl.html GPL Version 3
 * @author    Volker Theile <volker.theile@openmediavault.org>
 * @copyright Copyright (c) 2009-2016 Volker Theile
 *
 * OpenMediaVault is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * OpenMediaVault is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenMediaVault. If not, see <http://www.gnu.org/licenses/>.
 */
namespace OMV\Config;

require_once("openmediavault/globals.inc");

class Database extends \OMV\BaseObject {
	private $backend = NULL;

	public function __construct() {
		$this->backend = new DatabaseBackend($GLOBALS['OMV_CONFIG_FILE']);
		$this->backend->setVersioning(TRUE);
		$this->backend->load();
	}

	/**
	 * Returns the configuration database singleton.
	 * @return The singleton object.
	 */
	public static function &getInstance() {
		static $instance = NULL;
		if (!isset($instance))
			$instance = new Database();
		return $instance;
	}

	public function getBackend() {
		return $this->backend;
	}

	/**
	 * Get the configuration object at the specified XPath expression.
	 * @param xpath The XPath expression to execute.
	 * @param id The data model identifier. If \em id is not set, then
	 *   the xpath will be used as identifier instead.
	 * @return The configuration object.
	 */
	public function get($xpath, $id = "") {
		if (empty($id))
			$id = $xpath;
		$data = $this->getBackend()->get($xpath);
		if (!is_array($data)) {
			throw new DatabaseException("Failed to execute query '%s'.",
			  $xpath);
		}
		$object = new ConfigObject($id);
		$object->setAssoc($data);
		return $object;
	}

	/**
	 * Get the configuration objects at the specified XPath expression
	 * as an array.
	 * @param xpath The XPath expression to execute.
	 * @param id The data model identifier. If \em id is not set, then
	 *   the xpath will be used as identifier instead.
	 * @return An array containing the requested configuration objects,
	 *   otherwise an empty array.
	 */
	public function getList($xpath, $id = "") {
		if (empty($id))
			$id = $xpath;
		$data = $this->getBackend()->getList($xpath);
		if (!is_array($data)) {
			throw new DatabaseException("Failed to execute query '%s'.",
			  $xpath);
		}
		$objects = array();
		foreach ($data as $datak => $datav) {
			$object = new ConfigObject($id);
			$object->setAssoc($datav, FALSE);
			$objects[] = $object;
		}
		return $objects;
	}

	/**
	 * Set the configuration object at the specified XPath expression.
	 * @param xpath The XPath expression to execute.
	 * @param object The configuration object to use.
	 * @return None.
	 */
	public function set($xpath, ConfigObject $object) {
		$data = $object->getAssoc();
		if (FALSE === $this->getBackend()->set($xpath, $data))
			throw new DatabaseException("XPath '%s' does not exist.", $xpath);
	}

	/**
	 * Replace the configuration at the specified XPath expression.
	 * @param xpath The XPath expression to execute.
	 * @param object The configuration object to use.
	 * @return None.
	 */
	public function replace($xpath, ConfigObject $object) {
		$data = $object->getAssoc();
		if (FALSE === $this->getBackend()->replace($xpath, $data))
			throw new DatabaseException("XPath '%s' does not exist.", $xpath);
	}

	/**
	 * Update the existing configuration at the specified XPath expression.
	 * The specified data will override the existing configuration. If the
	 * given XPath expression does not exist, then the method will exit
	 * immediatelly.
	 * @param xpath The XPath expression to execute.
	 * @param object The configuration object to use.
	 * @return None.
	 */
	public function update($xpath, ConfigObject $object) {
		$data = $object->getAssoc();
		if (FALSE === $this->getBackend()->update($xpath, $data))
			throw new DatabaseException("XPath '%s' does not exist.", $xpath);
	}

	/**
	 * Delete the nodes matching the specified XPath expression.
	 * @param xpath The XPath expression to execute.
	 * @return None.
	 */
	public function delete($xpath) {
		if (FALSE === $this->getBackend()->delete($xpath))
			throw new DatabaseException("XPath '%s' does not exist.", $xpath);
	}
}