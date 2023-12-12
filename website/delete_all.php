<?php
//Connect to database
    session_start();
    require'connectDB.php';

    if (isset($_POST['log_date'])) {
      if ($_POST['date_sel'] != 0) {
          $_SESSION['seldate'] = $_POST['date_sel'];
      }
      else{
          $_SESSION['seldate'] = date("Y-m-d");
      }
    }
    
    if ($_POST['select_date'] == 1) {
        $_SESSION['seldate'] = date("Y-m-d");
    }
    else if ($_POST['select_date'] == 0) {
        $seldate = $_SESSION['seldate'];
    }

    $sql = "SELECT * FROM users_logs WHERE checkindate='$seldate' ORDER BY id DESC";
    $result = mysqli_stmt_init($conn);
    if (!mysqli_stmt_prepare($result, $sql)) {
        echo '<p class="error">SQL Error</p>';
    }
    else{
      mysqli_stmt_execute($result);
        $resultl = mysqli_stmt_get_result($result);
      if (mysqli_num_rows($resultl) > 0){
          while ($row = mysqli_fetch_assoc($resultl)){
                $id = $row['id'];
                $deleteQuery = "DELETE FROM users_logs WHERE id = '$id'";
                mysqli_query($conn, $deleteQuery);

          }   
      }
    }
  ?>


?>